// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <kernel/phys_alloc.h>

#include <kernel/mmap.h>
#include <kernel/page.h>
#include <kernel/panic.h>
#include <boot/symbols.h>
#include <io/kprint.h>

#include <lib/stdint.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <lib/itoa.h>

static uintptr_t g_bump_next;
static uintptr_t g_bump_end;

// TODO: This should probably be done in a file more related to paging/vmm
static void __init_phys_alloc_bump_map(void)
{
	uintptr_t next = g_bump_next;
	page_table_t *pml4 = (page_table_t *)&boot_pml4;
	page_table_t *pdpt, *pd, *pt;
	uint64_t pte;
	int pml4_idx = pml4_index(next);
	int pdpt_idx, pd_idx, pt_idx;

	// If there are problems with the pml4 or pdpt they should have been
	// addressed much earlier. This allocator should only have access to
	// RAM in low memory.
	pdpt = (page_table_t *)pte_addr(pml4->entries[pml4_idx]);
	if (!pte_is_present(pml4->entries[pml4_idx]))
		panic("phys_alloc_bump: PML4 entry not present");
	pdpt_idx = pdpt_index(next);
	if (pte_is_present(pdpt->entries[pdpt_idx]))
		panic("phys_alloc_bump: PDPT entry already present");

	// Create first entry, this will be enough to finish mapping the bump
	// allocator region. It is defined in start.S in .init.bss
	memset(&alloc_pd, 0, PAGE_SIZE_4K);
	memset(&alloc_pt, 0, PAGE_SIZE_4K);
	pte = pte_create((uintptr_t)&alloc_pd, PTE_KERNEL_RW);
	pdpt->entries[pdpt_idx] = pte;
	pd = &alloc_pd;
	pd->entries[0] = pte_create((uintptr_t)&alloc_pt, PTE_KERNEL_RW);
	pt = &alloc_pt;
	pt->entries[0] = pte_create(next, PTE_KERNEL_RW);
	next += PAGE_SIZE_4K;

	// Map remaining region
	while (next < g_bump_end) {
		pml4_idx = pml4_index(next);
		pdpt_idx = pdpt_index(next);
		pd_idx = pd_index(next);
		pt_idx = pt_index(next);
		pdpt = (page_table_t *)pte_addr(pml4->entries[pml4_idx]);
		if (!pte_is_present(pml4->entries[pml4_idx]))
			panic("phys_alloc_bump: PDPT entry not present");

		// Get or create PD
		if (!pte_is_present(pdpt->entries[pdpt_idx])) {
			uintptr_t new_pd_phys = phys_alloc_bump();
			if (new_pd_phys == NULL)
				panic("phys_alloc_bump: Unable to allocate PD");
			pd = (page_table_t *)new_pd_phys;
			memset(pd, 0, PAGE_SIZE_4K);
			pte = pte_create(new_pd_phys, PTE_KERNEL_RW);
			pdpt->entries[pdpt_idx] = pte;
		} else {
			pd = (page_table_t *)pte_addr(pdpt->entries[pdpt_idx]);
		}

		// Get or create PT
		if (!pte_is_present(pd->entries[pd_idx])) {
			uintptr_t new_pt_phys = phys_alloc_bump();
			if (new_pt_phys == NULL)
				panic("phys_alloc_bump: Unable to allocate PT");
			pt = (page_table_t *)new_pt_phys;
			memset(pt, 0, PAGE_SIZE_4K);
			pte = pte_create(new_pt_phys, PTE_KERNEL_RW);
			pd->entries[pd_idx] = pte;
		} else {
			pt = (page_table_t *)pte_addr(pd->entries[pd_idx]);
		}

		// Create next entry if not already present
		if (!pte_is_present(pt->entries[pt_idx])) {
			pte = pte_create((uintptr_t)next, PTE_KERNEL_RW);
			pt->entries[pt_idx] = pte;

		}
		next += PAGE_SIZE_4K;
	}
}

void __init_phys_alloc_bump(const struct pmmap *mmap)
{
	uintptr_t best_region_start = NULL;
	uintptr_t best_region_end = NULL;
	g_bump_next = NULL;
	g_bump_end = NULL;

	for (size_t i = 0; i < mmap->length; i++) {
		if (mmap->entries[i].type != PMMAP_TYPE_USABLE)
			continue;

		uintptr_t region_start = mmap->entries[i].base_paddress;
		uintptr_t region_end =
			mmap->entries[i].base_paddress + mmap->entries[i].length;

		if (region_end - region_start > best_region_end - best_region_start) {
			best_region_start = region_start;
			best_region_end = region_end;
		}
	}

	if (best_region_start == best_region_end) {
		panic("Unable to find a region of usable memory");
	}

	g_bump_next = best_region_start;
	g_bump_end = best_region_end;

	__init_phys_alloc_bump_map();
}

uintptr_t phys_alloc_bump(void)
{
	if (g_bump_next >= g_bump_end)
		panic("Bump allocator OOM");

	uintptr_t addr = g_bump_next;
	g_bump_next += PAGE_SIZE_4K;
	return addr;
}

uintptr_t phys_alloc_bump_n(size_t n)
{
	size_t size = n * PAGE_SIZE_4K;
	if ((g_bump_next + size) >= g_bump_end)
		return NULL;

	uintptr_t addr = g_bump_next;
	g_bump_next += size;
	return addr;
}

uintptr_t phys_page_alloc(void)
{
	return NULL;
}

void phys_page_free(uintptr_t page)
{
	if (page == NULL)
		return;
}

void phys_alloc_init(struct pmmap *mmap)
{
	(void)mmap;
}


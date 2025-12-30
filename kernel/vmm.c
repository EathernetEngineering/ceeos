// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <kernel/vmm.h>

#include <kernel/mmap.h>
#include <kernel/page.h>
#include <kernel/panic.h>
#include <kernel/phys_alloc.h>
#include <boot/symbols.h>
#include <io/kprint.h>

#include <lib/stdlib.h>
#include <lib/string.h>

static uintptr_t get_phys_max(void);
static uintptr_t alloc_page_table(void);
static page_table_t *ensure_table(page_table_t *parent, uint16_t idx, uint64_t flags);
static inline page_table_t *table_from_entry(page_table_entry_t e)
{
	return (page_table_t *)vmm_phys_to_virt(pte_addr(e));
}

static int __init_map_range(struct address_space *as,
		uintptr_t phys, uintptr_t virt,
		size_t len, uint64_t flags);
static int __init_map_kernel(void);
static int __init_map_phys(void);

static int g_bump_alloc;
static struct address_space g_kaddress_space;

int __init_paging(void)
{
	int ec;
	g_bump_alloc = 1;
	g_kaddress_space.pml4_phys = alloc_page_table();
	if (g_kaddress_space.pml4_phys == NULL)
		panic("Could not allocate page table");
	memset((uint8_t *)g_kaddress_space.pml4_phys, 0, PAGE_SIZE_4K);

	if ((ec = __init_map_phys()) != 0)
		return ec;

	if ((ec = __init_map_kernel()) != 0)
		return ec;

	pml4_set(g_kaddress_space.pml4_phys);

	return 0;
}

int vmm_map_range(struct address_space *as,
		uintptr_t vaddr, uintptr_t paddr, size_t length,
		uint64_t flags, enum page_size max_ps)
{
	size_t offset;
	if (length == 0 || ((length & (PAGE_SIZE_4K - 1)) != 0)) {
		return -1;
	}
	if (length > PAGE_SIZE_1G && max_ps >= PAGE_SIZE_1G_ENUM) {
		panic("1G pages not supported yet");
	}
	if (length > PAGE_SIZE_2M && max_ps >= PAGE_SIZE_2M_ENUM) {
		panic("2M pages not supported yet");
	}
	for (offset = 0; offset < length; offset += PAGE_SIZE_4K) {
		if (vmm_map_page(as, vaddr + offset, paddr + offset, flags, PAGE_SIZE_4K_ENUM) != 0)
			return -1;
	}
	return 0;
}

int vmm_map_page(struct address_space *as,
		uintptr_t vaddr, uintptr_t paddr,
		uint64_t flags, enum page_size ps)
{
	size_t page_size;
	switch (ps) {
		case PAGE_SIZE_4K_ENUM:
			page_size = PAGE_SIZE_4K;
			break;
		case PAGE_SIZE_2M_ENUM:
			page_size = PAGE_SIZE_2M;
			break;
		case PAGE_SIZE_1G_ENUM:
			page_size = PAGE_SIZE_1G;
			break;
		default:
			return -4; // Invalid page size
	}
	if (paddr & (page_size - 1))
		return -1;
	if (vaddr & (page_size - 1))
		return -1;
	page_table_t *pml4 = (page_table_t *)as->pml4_phys;
	if (pml4 == NULL)
		return -1;

	size_t pml4_idx = pml4_index(vaddr);
	page_table_t *pdpt = ensure_table(pml4, pml4_idx, PTE_RW | PTE_P);
	if (pdpt == NULL)
		return -2; // OOM

	size_t pdpt_idx = pdpt_index(vaddr);
	page_table_t *pd = ensure_table(pdpt, pdpt_idx, PTE_RW | PTE_P);
	if (pd == NULL)
		return -2; // OOM

	if (ps == PAGE_SIZE_1G_ENUM) {
		if (pdpt->entries[pdpt_idx] & PTE_P)
			return -3; // Already Mapped
		pdpt->entries[pdpt_idx] = pte_create(paddr, flags | PTE_P | PTE_PS);
		return 0;
	}

	size_t pd_idx = pd_index(vaddr);
	page_table_t *pt = ensure_table(pd, pd_idx, PTE_RW | PTE_P);
	if (pt == NULL)
		return -2; // OOM

	if (ps == PAGE_SIZE_2M_ENUM) {
		if (pd->entries[pd_idx] & PTE_P)
			return -3; // Already Mapped
		pd->entries[pd_idx] = pte_create(paddr, flags | PTE_P | PTE_PS);
		return 0;
	}

	size_t pt_idx = pt_index(vaddr);
	if (pt->entries[pt_idx] & PTE_P)
		return -3; // Already Mapped

	pt->entries[pt_idx] = pte_create(paddr, flags | PTE_P);

	return 0;
}

int vmm_unmap_page(struct address_space *as, uintptr_t vaddr)
{
	(void)as; // TODO: implement
	if (vaddr == NULL) {
		return -1;
	}

	return 0;
}

void vmm_handle_pf(const struct isr_context *ctx)
{
	(void)ctx;
	switch (ctx->error_code) {
		case 0:
			kprint("!!! Page fault: non-present page.\r\n");
			break;
		case 1:
			kprint("!!! Page fault: write access violation.\r\n");
			break;
		case 2:
			kprint("!!! Page fault: user-mode access violation.\r\n");
			break;
		case 4:
			kprint("!!! Page fault: reserved bit violation.\r\n");
			break;
		case 8:
			kprint("!!! Page fault: instruction fetch violation.\r\n");
			break;
		default:
			kprint("!!! Page fault: unknown error code.\r\n");
			break;
	}
	panic("Page fault handler not implemented");
}

static uintptr_t get_phys_max(void)
{
	uintptr_t physmax = NULL;
	const struct pmmap *mmap = mmap_get();
	for (long i = 0; i < (long)mmap->length; i++) {
		const struct pmmap_entry *e = &mmap->entries[i];
		physmax = max(physmax, e->base_paddress + e->length);
	}
	return physmax;
}

static uintptr_t alloc_page_table(void)
{
	if (g_bump_alloc)
		return phys_alloc_bump();
	else
		return phys_page_alloc();
}

static page_table_t *ensure_table(page_table_t *parent, uint16_t idx, uint64_t flags)
{
	page_table_entry_t e = parent->entries[idx];
	if (pte_is_present(e))
		return table_from_entry(e);

	uintptr_t phys = alloc_page_table();
	if (phys == NULL)
		return NULL;

	page_table_t *pt = vmm_phys_to_virt(phys);
	memset(pt, 0, 512);

	parent->entries[idx] = pte_create(phys, flags | PTE_P);

	return pt;
}

static int __init_map_range(struct address_space *as,
		uintptr_t phys, uintptr_t virt,
		size_t len, uint64_t flags)
{
	uintptr_t phys_max = phys + len;
	if (phys > phys_max)
		panic("Phys addr overflow in __init_map_range");
	page_table_t *pml4 = (page_table_t *)as->pml4_phys;
	while (phys < phys_max) {
		int pml4_idx = pml4_index(virt);
		int pdpt_idx = pdpt_index(virt);
		int pd_idx = pd_index(virt);
		int pt_idx = pt_index(virt);

		// Get or create PDPT
		if (!pte_is_present(pml4->entries[pml4_idx])) {
			uintptr_t new_pdpt = alloc_page_table();
			if (new_pdpt == NULL)
				return 1;
			memset((uint8_t *)new_pdpt, 0, PAGE_SIZE_4K);
			uint64_t pml4e = pte_create(new_pdpt, PTE_KERNEL_RW);
			pml4->entries[pml4_idx] = pml4e;
		}
		page_table_t *pdpt = (page_table_t *)pte_addr(pml4->entries[pml4_idx]);

		// Get or create PD
		if (!pte_is_present(pdpt->entries[pdpt_idx])) {
			page_table_t *new_pd = (page_table_t *)alloc_page_table();
			if (new_pd == NULL)
				return 1;
			memset(new_pd, 0, PAGE_SIZE_4K);
			uint64_t pdpte = pte_create((uintptr_t)new_pd, PTE_KERNEL_RW);
			pdpt->entries[pdpt_idx] = pdpte;
		}
		page_table_t *pd = (page_table_t *)pte_addr(pdpt->entries[pdpt_idx]);

		// Get or create PT
		if (!pte_is_present(pd->entries[pd_idx])) {
			page_table_t *new_pt = (page_table_t *)alloc_page_table();
			if (new_pt == NULL)
				return 1;
			memset(new_pt, 0, PAGE_SIZE_4K);
			uint64_t pde = pte_create((uintptr_t)new_pt, PTE_KERNEL_RW);
			pd->entries[pd_idx] = pde;
		}
		page_table_t *pt = (page_table_t *)pte_addr(pd->entries[pd_idx]);

		// Create next entry if not already present
		if (!pte_is_present(pt->entries[pt_idx])) {
			uint64_t pte = pte_create(phys, flags);
			pt->entries[pt_idx] = pte;
		} else {
			kprint("This shouldn't be mapped?");
		}
		phys += PAGE_SIZE_4K;
		virt += PAGE_SIZE_4K;
	}
	return 0;
}

static int __init_map_kernel(void)
{
	int ec;
	// Map entire boot trampoline as RWX
	uintptr_t boot_start = ALIGN_DOWN((uintptr_t)_boot_phys_start, PAGE_SIZE_4K);
	uintptr_t boot_end = ALIGN_UP((uintptr_t)_boot_phys_end, PAGE_SIZE_4K);
	ec =__init_map_range(&g_kaddress_space,
			boot_start,
			boot_start,
			boot_end - boot_start,
			PTE_KERNEL_RW);
	if (ec)
		panic("Failed to map boot segment");

	// Map in kernel segments

	uintptr_t kernel_text_start = ALIGN_DOWN((uintptr_t)_text_phys_start, PAGE_SIZE_4K);
	uintptr_t kernel_text_end = ALIGN_UP((uintptr_t)_text_phys_end, PAGE_SIZE_4K);
	ec = __init_map_range(&g_kaddress_space,
			kernel_text_start,
			(uintptr_t)(_kernel_virt_offset + kernel_text_start),
			(size_t)(kernel_text_end - kernel_text_start),
			PTE_KERNEL_RX);
	if (ec)
		panic("Failed to map kernel text segment");

	uintptr_t kernel_rodata_start = ALIGN_DOWN((uintptr_t)_rodata_phys_start, PAGE_SIZE_4K);
	uintptr_t kernel_rodata_end = ALIGN_UP((uintptr_t)_rodata_phys_end, PAGE_SIZE_4K);
	ec = __init_map_range(&g_kaddress_space,
			kernel_rodata_start,
			(uintptr_t)(_kernel_virt_offset + kernel_rodata_start),
			(size_t)(kernel_rodata_end - kernel_rodata_start),
			PTE_KERNEL_RX);
	if (ec)
		panic("Failed to map kernel rodata segment");

	uintptr_t kernel_data_start = ALIGN_DOWN((uintptr_t)_data_phys_start, PAGE_SIZE_4K);
	uintptr_t kernel_data_end = ALIGN_UP((uintptr_t)_data_phys_end, PAGE_SIZE_4K);
	ec = __init_map_range(&g_kaddress_space,
			kernel_data_start,
			(uintptr_t)(_kernel_virt_offset + kernel_data_start),
			(size_t)(kernel_data_end - kernel_data_start),
			PTE_KERNEL_RW);
	if (ec)
		panic("Failed to map kernel data segment");

	uintptr_t kernel_bss_start = ALIGN_DOWN((uintptr_t)_bss_phys_start, PAGE_SIZE_4K);
	uintptr_t kernel_bss_end = ALIGN_UP((uintptr_t)_bss_phys_end, PAGE_SIZE_4K);
	ec = __init_map_range(&g_kaddress_space,
			kernel_bss_start,
			(uintptr_t)(_kernel_virt_offset + kernel_bss_start),
			(size_t)(kernel_bss_end - kernel_bss_start),
			PTE_KERNEL_RW);
	if (ec)
		panic("Failed to map kernel bss segment");

	return 0;
}

static int __init_map_phys(void)
{
	// Build a real table
	return __init_map_range(&g_kaddress_space,
				0, PHYSMAP_BASE,
				get_phys_max(),
				PTE_KERNEL_RW | PTE_NX);
	
}


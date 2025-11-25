// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <mmap.h>

#include <panic.h>
#include <boot/multiboot2.h>
#include <io/kprint.h>

#include <itoa.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static struct multiboot_tag_efi_mmap *g_mb_efi_mmap;
static struct multiboot_tag_basic_meminfo *g_mb_basic_minfo;
static struct multiboot_tag_mmap *g_mb_mmap;

static struct pmmap g_mmap;

static void mmap_sort_addr(void);
static void mmap_merge_entries(void);
static int mmap_parse_mb_mmap(void);
static int mmap_parse_efi_mmap(void);
static int mmap_parse_mb_minfo(void);

void mmap_set_efi_mmap(struct multiboot_tag_efi_mmap *mmap)
{
	g_mb_efi_mmap = mmap;
}

void mmap_set_mb_basic_minfo(struct multiboot_tag_basic_meminfo *minfo)
{
	g_mb_basic_minfo = minfo;
}

void mmap_set_mb_mmap(struct multiboot_tag_mmap *mmap)
{
	g_mb_mmap = mmap;
}

int mmap_parse(void)
{
	int ec = 0, ret = 0;

	g_mmap.length = 0;

	ec = mmap_parse_mb_mmap();
	if (ec) {
		kprint("Failed to parse multiboot memory map\r\n");
		ret = -1;
	}
	ec = mmap_parse_efi_mmap();
	if (ec) {
		kprint("Failed to parse EFI memory map\r\n");
		ret = ret ? ret : 1;
	}
	if (g_mmap.length == 0) {
		ec = mmap_parse_mb_minfo();
		if (ec) {
			kprint("Failed to parse multiboot basic meminfo\r\n");
			panic();
		}
	}
	mmap_normalize(NULL);
	if (g_mmap.length == 0)
		ret = -1;

	return ret;
}

const struct pmmap *mmap_get(void)
{
	return &g_mmap;
}

int mmap_swap_entries(int lhs, int rhs)
{
	struct pmmap_entry tmp;
	if ((size_t)lhs >= g_mmap.length || (size_t)rhs >= g_mmap.length)
		return -1;

	memcpy(&tmp, &g_mmap.entries[lhs], sizeof(struct pmmap_entry));
	memcpy(&g_mmap.entries[lhs], &g_mmap.entries[rhs],
		sizeof(struct pmmap_entry));
	memcpy(&g_mmap.entries[rhs], &tmp, sizeof(struct pmmap_entry));

	return 0;
}

int mmap_remove_entry(int i)
{
	if ((size_t)i >= g_mmap.length)
		return -1;

	if (i != (int)g_mmap.length - 1) {
		size_t remaining = g_mmap.length - (i + 1);
		memmove(&g_mmap.entries[i], &g_mmap.entries[i + 1],
			 remaining * sizeof(struct pmmap_entry));
	}
	g_mmap.length--;
	memset(&g_mmap.entries[g_mmap.length], 0, sizeof(struct pmmap_entry));

	return 0;
}

int mmap_insert_entry(int i, const struct pmmap_entry *e)
{
	if (g_mmap.length >= PMMAP_MAX_ENTRIES || i > (int)g_mmap.length)
		return -1;

	if (i != (int)g_mmap.length) {
		int remaining = (int)g_mmap.length - (i + 1);
		memmove(&g_mmap.entries[i + 1], &g_mmap.entries[i],
			remaining * sizeof(struct pmmap_entry));
	}
	memcpy(&g_mmap.entries[i], e, sizeof(struct pmmap_entry));
	g_mmap.length++;

	return 0;
}

int mmap_append_entry(const struct pmmap_entry *e)
{
	return mmap_insert_entry(g_mmap.length, e);
}

void mmap_normalize(int *diff)
{
	int inital_count = g_mmap.length;
	mmap_sort_addr();
	mmap_merge_entries();
	if (diff)
		*diff = (int)g_mmap.length - inital_count;
}

static void mmap_sort_addr(void)
{
	for (long i = 0; (size_t)i < g_mmap.length - 1; i++) {
		int swapped = 0;
		for (long j = 0; j < (long)g_mmap.length - i - 1; j++) {
			if (g_mmap.entries[j].base_paddress >
				g_mmap.entries[j + 1].base_paddress)
			{
				mmap_swap_entries(j, j + 1);
				swapped = 1;
			}
		}
		if (!swapped)
			break;
	}
}

static void mmap_merge_entries(void)
{
	for (long i = 0; (size_t)i < g_mmap.length - 1; i++) {
		struct pmmap_entry *thise = &g_mmap.entries[i];
		struct pmmap_entry *nexte = &g_mmap.entries[i + 1];
		uintptr_t this_end_addr = thise->base_paddress + thise->length;
		uintptr_t next_end_addr = nexte->base_paddress + nexte->length;

		if (thise->type == nexte->type &&
			nexte->base_paddress <= this_end_addr &&
			thise->base_paddress == thise->base_vaddress &&
			nexte->base_paddress == nexte->base_vaddress)
		{
			this_end_addr = max(this_end_addr, next_end_addr);
			thise->length = this_end_addr - thise->base_paddress;
			mmap_remove_entry(i + 1);
			i--;
		}
	}
}

static int mmap_parse_mb_mmap(void)
{
	multiboot_memory_map_t *entry;
#ifndef NDEBUG
	if (g_mb_mmap->type != MULTIBOOT_TAG_TYPE_MMAP) {
		kprint("Unexpected tag provided to mb_mmap_parse()\r\n");
		panic();
	}
#endif
	multiboot_memory_map_t *entries_end =
		(multiboot_memory_map_t *)((uintptr_t)g_mb_mmap + g_mb_mmap->size);
	size_t entry_size = (size_t)g_mb_mmap->entry_size;
	int i = g_mmap.length;
	if (g_mb_mmap->entries >= entries_end)
		return -1;
	for (entry = g_mb_mmap->entries; entry < entries_end;
		entry = (multiboot_memory_map_t *)((uintptr_t)entry + entry_size))
	{
		struct pmmap_entry new_entry;
		new_entry.base_paddress = entry->addr;
		new_entry.base_vaddress = entry->addr;
		new_entry.length = entry->len;
		switch (entry->type) {
			case MULTIBOOT_MEMORY_AVAILABLE:
				new_entry.type = PMMAP_TYPE_USABLE;
				break;

			case MULTIBOOT_MEMORY_RESERVED:
				new_entry.type = PMMAP_TYPE_RESERVED;
				break;

			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
				new_entry.type = PMMAP_TYPE_ACPI_RECLAIMABLE;
				break;

			case MULTIBOOT_MEMORY_NVS:
				new_entry.type = PMMAP_TYPE_ACPI_NVS;
				break;

			case MULTIBOOT_MEMORY_BADRAM:
				new_entry.type = PMMAP_TYPE_BAD;
				break;

			default:
				new_entry.type = PMMAP_TYPE_RESERVED;
		}
		mmap_append_entry(&new_entry);
		++i;
		if (i >= PMMAP_MAX_ENTRIES) {
			int diff;
			g_mmap.length = i;
			mmap_normalize(&diff);
			if (diff >= 0) {
				char str[8];
				int str_length;
				kprint("!!! Truncated mmap to ");
				str_length = itoa(PMMAP_MAX_ENTRIES, str, 10);
				str[str_length] = '\0';
				kprint(str);
				kprint(" entries\r\n");
				return 1;
			}
			i = g_mmap.length;
		}
	}
	g_mmap.length = i;
	return 0;
}

static int mmap_parse_efi_mmap(void)
{
	struct efi_mmap_entry *entry;
#ifndef NDEBUG
	if (g_mb_efi_mmap->type != MULTIBOOT_TAG_TYPE_EFI_MMAP) {
		kprint("Unexpected tag provided to mb_efi_mmap_parse()\r\n");
		panic();
	}
#endif
	struct efi_mmap_entry *entries_end =
		(struct efi_mmap_entry *)((uintptr_t)g_mb_efi_mmap + g_mb_efi_mmap->size);
	size_t entry_size = (size_t)g_mb_efi_mmap->descr_size;
	int i = g_mmap.length;
	if ((uintptr_t)g_mb_efi_mmap->efi_mmap >= (uintptr_t)entries_end)
		return -1;
	for (entry = (struct efi_mmap_entry *)g_mb_efi_mmap->efi_mmap;
		entry < entries_end;
		entry = (struct efi_mmap_entry *)((uintptr_t)entry + entry_size))
	{
		struct pmmap_entry new_entry;
		new_entry.base_paddress = entry->paddr_start;
		// Dont care about the vaddr of anything that will never use it
		new_entry.base_vaddress = entry->paddr_start;
		new_entry.length = entry->page_count * 0x1000;
		switch (entry->type) {
			case EFI_RESERVED_MEMORY_TYPE:
			case EFI_PERSISTENT_MEMORY:
				new_entry.type = PMMAP_TYPE_RESERVED;
				break;

			case EFI_LOADER_CODE:
			case EFI_LOADER_DATA:
			case EFI_BOOT_SERVICES_CODE:
			case EFI_BOOT_SERVICES_DATA:
			case EFI_CONVENTIONAL_MEMORY:
				new_entry.type = PMMAP_TYPE_USABLE;
				break;

			case EFI_RUNTIME_SERVICES_CODE:
			case EFI_PAL_CODE:
				new_entry.type = PMMAP_TYPE_EFI_RUNTIME_CODE;
				new_entry.base_vaddress = entry->vaddr_start;
				break;

			case EFI_RUNTIME_SERVICES_DATA:
				new_entry.type = PMMAP_TYPE_EFI_RUNTIME_DATA;
				new_entry.base_vaddress = entry->vaddr_start;
				break;

			case EFI_ACPI_RECLAIM_MEMORY:
				new_entry.type = PMMAP_TYPE_ACPI_RECLAIMABLE;
				break;

			case EFI_ACPI_MEMORY_NVS:
				new_entry.type = PMMAP_TYPE_ACPI_NVS;
				break;

			case EFI_MEMORY_MAPPED_IO:
			case EFI_MEMORY_MAPPED_IO_PORT_SPACE:
				new_entry.type = PMMAP_TYPE_MMIO;
				break;

			case EFI_UNACCEPTED_MEMORY_TYPE:
				new_entry.type = PMMAP_TYPE_UNACCEPTED;
				break;

			case EFI_UNUSABLE_MEMORY:
				new_entry.type = PMMAP_TYPE_BAD;
				break;
		}
		mmap_append_entry(&new_entry);
		++i;
		if (i >= PMMAP_MAX_ENTRIES) {
			int diff;
			g_mmap.length = i;
			mmap_normalize(&diff);
			if (diff >= 0) {
				char str[8];
				int str_length;
				kprint("!!! Truncated mmap to ");
				str_length = itoa(PMMAP_MAX_ENTRIES, str, 10);
				str[str_length] = '\0';
				kprint(str);
				kprint(" entries\r\n");
				return 1;
			}
			i = g_mmap.length;
		}
	}
	g_mmap.length = i;
	return 0;
}

static int mmap_parse_mb_minfo(void)
{
#ifndef NDEBUG
	if (g_mb_basic_minfo->type != MULTIBOOT_TAG_TYPE_BASIC_MEMINFO) {
		kprint("Unexpected tag provided to mb_basic_meminfo_parse()\r\n");
		panic();
	}
#endif
	if (g_mmap.length != 0)
		return -1;

	g_mmap.length = 1;
	g_mmap.entries[0].type = PMMAP_TYPE_USABLE;
	g_mmap.entries[0].base_paddress = g_mb_basic_minfo->mem_lower;
	g_mmap.entries[0].base_vaddress = g_mb_basic_minfo->mem_lower;
	g_mmap.entries[0].length = g_mb_basic_minfo->mem_upper -
		g_mb_basic_minfo->mem_lower;

	kprint("Falling back to basic meminfo\r\n");

	return 0;
}


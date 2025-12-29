// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_MMAP_H_
#define CEEK_MMAP_H_

#include <boot/multiboot2.h>
#include <boot/efi.h>
#include <stdint.h>

#define PMMAP_MAX_ENTRIES 128

enum pmmap_entry_type {
	PMMAP_TYPE_BAD = 0,
	PMMAP_TYPE_USABLE = 1,
	PMMAP_TYPE_RESERVED = 2,
	PMMAP_TYPE_ACPI_RECLAIMABLE = 3,
	PMMAP_TYPE_ACPI_NVS = 4,
	PMMAP_TYPE_EFI_RUNTIME_CODE = 5,
	PMMAP_TYPE_EFI_RUNTIME_DATA = 6,
	PMMAP_TYPE_MMIO = 7,
	PMMAP_TYPE_UNACCEPTED = 8,
	PMMAP_TYPE_KERNEL_RESERVED = 9
};

struct pmmap_entry {
	uintptr_t base_paddress;
	uintptr_t base_vaddress;
	size_t length;
	enum pmmap_entry_type type;
};

struct pmmap {
	size_t length;
	struct pmmap_entry entries[PMMAP_MAX_ENTRIES];
};

int __init_mmap_parse(struct multiboot_tag_basic_meminfo *g_mb_basic_minfo,
		struct multiboot_tag_mmap *g_mb_mmap,
		struct multiboot_tag_efi_mmap *g_mb_efi_mmap);

const struct pmmap *mmap_get(void);

int mmap_swap_entries(int lhs, int rhs);
int mmap_remove_entry(int i);
int mmap_insert_entry(int i, const struct pmmap_entry *e);
int mmap_append_entry(const struct pmmap_entry *e);
void mmap_normalize(int *diff);

#endif


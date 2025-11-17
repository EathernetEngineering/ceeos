// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <mmap.h>

struct multiboot_tag_efi_mmap *multiboot_efi_mmap;
struct multiboot_tag_basic_meminfo *multiboot_minfo;
struct multiboot_tag_mmap *multiboot_mmap;

void mmap_set_efi_mmap(struct multiboot_tag_efi_mmap *mmap)
{
	multiboot_efi_mmap = mmap;
}

void mmap_set_mb_basic_minfo(struct multiboot_tag_basic_meminfo *minfo)
{
	multiboot_minfo = minfo;
}

void mmap_set_mb_mmap(struct multiboot_tag_mmap *mmap)
{
	multiboot_mmap = mmap;
}

int mmap_parse(void)
{
	// TODO: Parse the memory map
	return 0;
}


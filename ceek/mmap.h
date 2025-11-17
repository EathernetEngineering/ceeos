// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_MMAP_H_
#define CEEK_MMAP_H_

#include <boot/multiboot2.h>
#include <stdint.h>

enum memory_section_type {
	PMEM_TYPE_RESERVED = 0
};

struct memory_section {
	struct memory_section *prev, *next;
	uintptr_t base_address;
	size_t length;
	enum memory_section_type type;
};

struct pmem_map {
	size_t length;
	struct memory_section *begin;
};

void mmap_set_efi_mmap(struct multiboot_tag_efi_mmap *mmap);
void mmap_set_mb_basic_minfo(struct multiboot_tag_basic_meminfo *minfo);
void mmap_set_mb_mmap(struct multiboot_tag_mmap *mmap);

int mmap_parse(void);

#endif


// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_BOOT_PARSE_BM_INFO_H_
#define CEEK_BOOT_PARSE_BM_INFO_H_

#include <kernel/elf.h>

#include <lib/stdint.h>
#include <boot/multiboot2.h>

struct fb_info {
	uintptr_t phys_addr;
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint8_t bpp;
	uint8_t type;
	union {
		struct {
			uint8_t red_mask_size;
			uint8_t red_mask_shift;
			uint8_t green_mask_size;
			uint8_t green_mask_shift;
			uint8_t blue_mask_size;
			uint8_t blue_mask_shift;
		} rgb;
		struct {
			uint8_t palette_num_colors;
			void *palette;
		} indexed;
	};
};

struct section_info {
	uint16_t num;
	uint16_t entry_size;
	uint16_t shndx;
	struct elf_section_header *sections;
};

struct boot_info {
	const char *cmdline;
	const char *boot_loader_name;
	void *basic_meminfo;
	void *mmap;
	void *efi_mmap;
	uintptr_t rsdpv1;
	uintptr_t rsdpv2;
	struct fb_info framebuffer;
	struct section_info elf_sections;
};

int parse_boot_info(uintptr_t addr, struct boot_info *info);

#endif


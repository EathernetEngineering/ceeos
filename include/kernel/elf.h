// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_ELF_H_
#define CEEK_ELF_H_

#include <lib/stdint.h>

struct elf_header {
	uint8_t     e_ident[16];
	uint16_t    e_type;
	uint16_t    e_machine;
	uint32_t    e_version;
	uintptr_t   e_entry;
	uintptr_t   e_phoff;
	uintptr_t   e_shoff;
	uint32_t    e_flags;
	uint16_t    e_ehsize;
	uint16_t    e_phentsize;
	uint16_t    e_phnum;
	uint16_t    e_shentsize;
	uint16_t    e_shnum;
	uint16_t    e_shstrndx;
};

struct elf_program_header {
	uint32_t    p_type;
	uint32_t    p_flags;
	uintptr_t   p_offset;
	uintptr_t   p_vaddr;
	uintptr_t   p_paddr;
	uintptr_t   p_filesz;
	uintptr_t   p_memsz;
	uintptr_t   p_align;
};

struct elf_section_header {
	uint32_t    sh_name;
	uint32_t    sh_type;
	uintptr_t   sh_flags;
	uintptr_t   sh_addr;
	uintptr_t   sh_offset;
	uintptr_t   sh_size;
	uint32_t    sh_link;
	uint32_t    sh_info;
	uintptr_t   sh_addralign;
	uintptr_t   sh_entsize;
};

struct elf_symbol {
	uint32_t    st_name;
	uint8_t     st_info;
	uint8_t     st_other;
	uint16_t    st_shndx;
	uintptr_t   st_value;
	uintptr_t   st_size;
};

#endif


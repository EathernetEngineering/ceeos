// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_BOOT_SYMBOLS_H_
#define CEEK_BOOT_SYMBOLS_H_

#include <kernel/page.h>

// Page tables from boot
extern page_table_t boot_pml4;
extern page_table_t boot_pdpt_low;
extern page_table_t boot_pdpt_high;
extern page_table_t boot_pd_1g;
extern page_table_t boot_pd_2g;
extern page_table_t boot_pt_end;

extern page_table_t alloc_pd;
extern page_table_t alloc_pt;

extern uint8_t kernel_stack_bottom[];
extern uint8_t kernel_stack_top[];

// Useful symbols from linker script.
extern uint8_t _boot_text_start[];
extern uint8_t _boot_text_end[];
extern uint8_t _boot_rodata_start[];
extern uint8_t _boot_rodata_end[];
extern uint8_t _boot_bss_start[];
extern uint8_t _boot_bss_end[];
extern uint8_t _text_start[];
extern uint8_t _text_end[];
extern uint8_t _rodata_start[];
extern uint8_t _rodata_end[];
extern uint8_t _data_start[];
extern uint8_t _data_end[];
extern uint8_t _bss_start[];
extern uint8_t _bss_end[];

extern uint8_t _boot_text_phys_start[];
extern uint8_t _boot_text_phys_end[];
extern uint8_t _boot_rodata_phys_start[];
extern uint8_t _boot_rodata_phys_end[];
extern uint8_t _boot_bss_phys_start[];
extern uint8_t _boot_bss_phys_end[];
extern uint8_t _text_phys_start[];
extern uint8_t _text_phys_end[];
extern uint8_t _rodata_phys_start[];
extern uint8_t _rodata_phys_end[];
extern uint8_t _data_phys_start[];
extern uint8_t _data_phys_end[];
extern uint8_t _bss_phys_start[];
extern uint8_t _bss_phys_end[];

extern uint8_t _kernel_virt_base[];
extern uint8_t _kernel_virt_end[];
extern uint8_t _boot_phys_start[];
extern uint8_t _boot_phys_end[];
extern uint8_t _kernel_phys_start[];
extern uint8_t _kernel_phys_end[];
extern uint8_t _kernel_virt_offset[];

static inline size_t _boot_text_size(void)
{
	return (size_t)((uintptr_t)_boot_text_end - (uintptr_t)_boot_text_start);
}

static inline size_t _boot_rodata_size(void)
{
	return (size_t)((uintptr_t)_boot_rodata_end - (uintptr_t)_boot_rodata_start);
}

static inline size_t _boot_bss_size(void)
{
	return (size_t)((uintptr_t)_boot_bss_end - (uintptr_t)_boot_bss_start);
}

static inline size_t _text_size(void)
{
	return (size_t)((uintptr_t)_text_end - (uintptr_t)_text_start);
}

static inline size_t _rodata_size(void)
{
	return (size_t)((uintptr_t)_rodata_end - (uintptr_t)_rodata_start);
}

static inline size_t _data_size(void)
{
	return (size_t)((uintptr_t)_data_end - (uintptr_t)_data_start);
}

static inline size_t _bss_size(void)
{
	return (size_t)((uintptr_t)_text_end - (uintptr_t)_text_start);
}

static inline size_t _boot_text_phys_size(void)
{
	return (size_t)((uintptr_t)_boot_text_phys_end - (uintptr_t)_boot_text_phys_start);
}

static inline size_t _boot_rodata_phys_size(void)
{
	return (size_t)((uintptr_t)_boot_rodata_phys_end - (uintptr_t)_boot_rodata_phys_start);
}

static inline size_t _boot_bss_phys_size(void)
{
	return (size_t)((uintptr_t)_boot_bss_phys_end - (uintptr_t)_boot_bss_phys_start);
}

static inline size_t _text_phys_size(void)
{
	return (size_t)((uintptr_t)_text_phys_end - (uintptr_t)_text_phys_start);
}

static inline size_t _rodata_phys_size(void)
{
	return (size_t)((uintptr_t)_rodata_phys_end - (uintptr_t)_rodata_phys_start);
}

static inline size_t _data_phys_size(void)
{
	return (size_t)((uintptr_t)_data_phys_end - (uintptr_t)_data_phys_start);
}

static inline size_t _bss_phys_size(void)
{
	return (size_t)((uintptr_t)_text_phys_end - (uintptr_t)_text_phys_start);
}
#endif


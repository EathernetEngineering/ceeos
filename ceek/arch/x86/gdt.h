// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_GDT_H_
#define CEEK_GDT_H_

#include <stdint.h>

struct gdt_descriptor {
	uint16_t size;
	void *entrites;
};

void init_gdt(void);

uint16_t gdt_get_interrupt_data_segment(void);
uint16_t gdt_get_interrupt_code_segment(void);

uint16_t gdt_get_kernel_identity_data_segment(void);
uint16_t gdt_get_kernel_identity_code_segment(void);

uint16_t gdt_get_user_identity_data_segment(void);
uint16_t gdt_get_user_identity_code_segment(void);

static inline void set_gdtr(struct gdt_descriptor *d)
{
	asm volatile ("lgdt %0" :: "m"(*d));
}

#endif


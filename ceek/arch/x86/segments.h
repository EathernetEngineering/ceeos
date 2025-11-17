// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_X86_SEGMENTS_H_
#define CEEK_X86_SEGMENTS_H_

#include <stdint.h>

#define SEGMENT_ACCESS_A                 0x01
#define SEGMENT_ACCESS_RW                0x02
#define SEGMENT_ACCESS_DC                0x04
#define SEGMENT_ACCESS_E                 0x08
#define SEGMENT_ACCESS_S                 0x10
#define SEGMENT_ACCESS_DPL_0             0x00
#define SEGMENT_ACCESS_DPL_1             0x20
#define SEGMENT_ACCESS_DPL_2             0x40
#define SEGMENT_ACCESS_DPL_3             0x60
#define SEGMENT_ACCESS_P                 0x80

#define SYS_SEGMENT_ACCESS_LDT           0x02
#define SYS_SEGMENT_ACCESS_TSS_AVAILABLE 0x09
#define SYS_SEGMENT_ACCESS_TSS_BUSY      0x0B
#define SYS_SEGMENT_ACCESS_S             0x10
#define SYS_SEGMENT_ACCESS_DPL_0         0x00
#define SYS_SEGMENT_ACCESS_DPL_1         0x20
#define SYS_SEGMENT_ACCESS_DPL_2         0x40
#define SYS_SEGMENT_ACCESS_DPL_3         0x60
#define SYS_SEGMENT_ACCESS_P             0x80

#define SEGMENT_FLAG_L                   0x20
#define SEGMENT_FLAG_DB                  0x40
#define SEGMENT_FLAG_G                   0x80

struct gdte {
	uint16_t limit_lower;
	uint16_t base_lower;
	uint8_t base_middle;
	uint8_t access;
	uint8_t flags_limit_upper;
	uint8_t base_upper;
};

struct gdte_long {
	struct gdte base;
	uint32_t base_higher;
	uint32_t reserved;
};
_Static_assert(sizeof(struct gdte_long) == 2 * sizeof(struct gdte),
			   "Long mode segment descriptors must be 2 usual ones");

struct __attribute__((__packed__)) gdt_descriptor {
	uint16_t size;
	struct gdte *location;
};

static inline uint32_t gdte_get_limit(const struct gdte *entry)
{
	uint32_t limit = 0;
	limit |= (uint32_t)(entry->flags_limit_upper & 0x0F) << 24;
	limit |= (uint32_t)entry->limit_lower;
	return limit;
}

struct gdte *gdte_set_limit(struct gdte *entry, uint32_t addr);

static inline uint32_t gdte_get_base(const struct gdte *entry)
{
	uint32_t base = 0;
	base |= (uint32_t)entry->base_upper  << 24;
	base |= (uint32_t)entry->base_middle << 16;
	base |= (uint32_t)entry->base_lower;
	return base;
}

static inline struct gdte *gdte_set_base(struct gdte *entry, uint32_t addr)
{
	entry->base_upper  = (uint8_t)(addr >> 24);
	entry->base_middle = (uint8_t)(addr >> 16);
	entry->base_lower  = (uint16_t)addr;
	return entry;
}

static inline struct gdte_long
	*gdte_long_set_base(struct gdte_long *entry, uintptr_t addr)
{
	entry->base_higher = (uint16_t)(addr >>  32);
	entry->base.base_upper  = (uint8_t)(addr >> 24);
	entry->base.base_middle = (uint8_t)(addr >> 16);
	entry->base.base_lower  = (uint16_t)addr;
	return entry;
}

void gdt_init(void);

uint16_t gdt_get_interrupt_identiry_data_segment(void);
uint16_t gdt_get_interrupt_identity_code_segment(void);

uint16_t gdt_get_kernel_identity_data_segment(void);
uint16_t gdt_get_kernel_identity_code_segment(void);

uint16_t gdt_get_user_identity_data_segment(void);
uint16_t gdt_get_user_identity_code_segment(void);

extern void gdtr_set(struct gdt_descriptor *d, uint16_t cs, uint16_t ss,
					 uint16_t ds, uint16_t es);

#endif


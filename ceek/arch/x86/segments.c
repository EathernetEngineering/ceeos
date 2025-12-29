// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/segments.h>

#include <io/kprint.h>
#include <string.h>
#include <stddef.h>

/*
 * Initial mapping:
 * 0x00: Null
 * 0x08: kernel identity code segment
 * 0x10: kernel identity data segment
 * 0x18: user identity code segment
 * 0x20: user identity data segment
 * 0x30: kernel tss
 */
static struct gdt_descriptor g_gdt_desc;
static struct gdte g_gdt[64];

struct gdte *gdte_set_limit(struct gdte *entry, uint32_t addr)
{
#ifndef NDEBUG
	if (addr & ~0xFFFFFu) {
		kprint("GDT_SET_LIMIT Expects a 20 bit value");
	}
#endif
	entry->flags_limit_upper &= 0xF0;
	entry->flags_limit_upper |= (uint8_t)((addr >> 16) & 0x0F);
	entry->limit_lower        = (uint16_t)(addr >>  0);
	return entry;
}

void __init_gdt(void)
{
	struct gdte entry;
	memset(&entry, 0, sizeof(entry));
	memset(g_gdt, 0, sizeof(g_gdt)/sizeof(g_gdt[0]));

	/*
	 *  The first segment must be null, then 4 segments will be identity
	 *  mapped. This will be useful while parsing a memory map before
	 *  setting up the "real" gdt.
	 */
	gdte_set_limit(&entry, 0xFFFFF); // Bits 0-20 set;
	gdte_set_base(&entry, 0);

	entry.flags_limit_upper |= SEGMENT_FLAG_G | SEGMENT_FLAG_L;
	entry.access = SEGMENT_ACCESS_RW | SEGMENT_ACCESS_E | SEGMENT_ACCESS_S |
		SEGMENT_ACCESS_P;
	memcpy(&g_gdt[1], &entry, sizeof(entry));

	entry.flags_limit_upper |= SEGMENT_FLAG_G;
	entry.access = SEGMENT_ACCESS_RW | SEGMENT_ACCESS_S | SEGMENT_ACCESS_P;
	memcpy(&g_gdt[2], &entry, sizeof(entry));

	entry.flags_limit_upper |= SEGMENT_FLAG_G | SEGMENT_FLAG_L;
	entry.access = SEGMENT_ACCESS_RW | SEGMENT_ACCESS_E | SEGMENT_ACCESS_S |
		SEGMENT_ACCESS_P | SEGMENT_ACCESS_DPL_3;
	memcpy(&g_gdt[3], &entry, sizeof(entry));

	entry.flags_limit_upper |= SEGMENT_FLAG_G;
	entry.access = SEGMENT_ACCESS_RW | SEGMENT_ACCESS_P | SEGMENT_ACCESS_S |
		SEGMENT_ACCESS_DPL_3;
	memcpy(&g_gdt[4], &entry, sizeof(entry));

	g_gdt_desc.location = g_gdt;
	g_gdt_desc.size = 5 * sizeof(entry) - 1;
	gdtr_set(&g_gdt_desc, 0x08, 0x10, 0x10, 0x10);
}

uint16_t gdt_get_interrupt_identity_data_segment(void)
{
	return 0x10;
}

uint16_t gdt_get_interrupt_identity_code_segment(void)
{
	return 0x08;
}

uint16_t gdt_get_kernel_identity_data_segment(void)
{
	return 0x10;
}

uint16_t gdt_get_kernel_identity_code_segment(void)
{
	return 0x08;
}

uint16_t gdt_get_user_identity_data_segment(void)
{
	return 0x20;
}

uint16_t gdt_get_user_identity_code_segment(void)
{
	return 0x18;
}


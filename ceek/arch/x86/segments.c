// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/segments.h>

void init_gdt(void)
{
}

uint16_t gdt_get_interrupt_data_segment(void)
{
	return 0x20;
}

uint16_t gdt_get_interrupt_code_segment(void)
{
	return 0x18;
}

uint16_t gdt_get_kernel_identity_data_segment(void)
{
	return 0x20;
}

uint16_t gdt_get_kernel_identity_code_segment(void)
{
	return 0x18;
}

uint16_t gdt_get_user_identity_data_segment(void)
{
	return 0x20;
}

uint16_t gdt_get_user_identity_code_segment(void)
{
	return 0x18;
}


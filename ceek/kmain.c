// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/segments.h>
#include <arch/x86/interrupts.h>
#include <io/kprint.h>

#include <panic.h>

void kernel_main(unsigned int magic, void *multiboot_info) {
	(void)magic, (void)multiboot_info;
	if (magic != 0x36d76289) { // multiboot2 magic number
		kprint("Unexpected state from bootloader!\r\n");
		panic();
	}
	kprint("Initializing GDT ...\r\n");
	init_gdt();
	kprint("... GDT initialized\r\n");
	kprint("Initializing IDT ...\r\n");
	init_idt();
	kprint("... IDT initialized\r\n");

	kprint("End of execution reached, entering eternal spin...\r\n");
	fatal_spin();

	__builtin_unreachable();
}


// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/segments.h>
#include <arch/x86/interrupts.h>
#include <io/kprint.h>

#include <boot/multiboot2.h>
#include <boot/parse_mb_info.h>

#include <panic.h>
#include <acpi.h>
#include <mmap.h>

#include <atoi.h>

void kernel_main(unsigned int magic, uintptr_t multiboot_info) {
	int ec;
	if (magic != 0x36d76289) { // multiboot2 magic number
		kprint("!!! Unexpected state from bootloader!\r\n");
		fatal_spin();
	}
	kprint("Seting up exception handlers\r\n");
	idt_init();
	kprint("Set up exception handlers: OK\r\n");
	gdt_init();

	/*
	 * Set these to NULL so when parsing whether or not the info was provided
	 * by the bootloader is known if .bss was not zero initialized
	 */
	mmap_set_mb_basic_minfo((void *)0);
	mmap_set_mb_mmap((void *)0);
	mmap_set_efi_mmap((void *)0);

	kprint("Parsing multiboot info...\r\n");
	if ((ec = parse_boot_info(multiboot_info))) {
		kprint("!!! Failed to parse multiboot information struct!");
		fatal_spin();
	}
	kprint("Parsed multiboot info: OK\r\n");
	if ((ec = mmap_parse())) {
		kprint("!!! Failed to get parse memory map!");
		fatal_spin();
	}
	if ((ec = acpi_init())) {
		kprint("!!! Failed to get parse ACPI info!");
		fatal_spin();
	}

	kprint("End of execution reached, entering eternal spin...\r\n");
	fatal_spin();
}


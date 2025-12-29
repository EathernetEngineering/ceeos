// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/segments.h>
#include <arch/x86/interrupts.h>
#include <io/kprint.h>

#include <boot/multiboot2.h>
#include <boot/parse_mb_info.h>
#include <boot/symbols.h>

#include <acpi.h>
#include <mmap.h>
#include <panic.h>
#include <phys_alloc.h>
#include <vmm.h>

#include <itoa.h>
#include <string.h>

void __init(uintptr_t multiboot_info_addr) {
	int ec;
	struct boot_info binfo;

	kprint("Seting up exception handlers\r\n");
	__init_idt();
	kprint("Set up exception handlers: OK\r\n");
	__init_gdt();

	memset(&binfo, 0, sizeof(struct boot_info));
	kprint("Parsing multiboot info...\r\n");
	if ((ec = parse_boot_info(multiboot_info_addr, &binfo)))
		panic("Failed to parse multiboot information struct!");

	kprint("Parsed multiboot info: OK\r\n");
	if (binfo.boot_loader_name) {
		kprint("Bootloader name: ");
		kprint(binfo.boot_loader_name);
		kprint("\r\n");
	}
	if (binfo.cmdline) {
		kprint("Kernel command line: ");
		kprint(binfo.cmdline);
		kprint("\r\n");
	}
	if ((ec = __init_mmap_parse(binfo.basic_meminfo, binfo.mmap, binfo.efi_mmap)))
		panic("Failed to parse memory map!");

	__init_phys_alloc_bump(mmap_get());
	if ((ec = __init_paging()))
		panic("Failed to initialize paging!");

	// if ((ec = acpi_init(binfo.rsdpv1, binfo.rsdpv2))) {
	// 	kprint("!!! Failed to parse ACPI info!");
	// 	fatal_spin();
	// }
}

void kernel_main(unsigned int magic, uintptr_t multiboot_info) {
	if (magic != 0x36d76289) // multiboot2 magic number
		panic("Unexpected state from bootloader!");

	__init(multiboot_info);

	kprint("End of execution reached, entering eternal spin...\r\n");
	spin_lock();
}


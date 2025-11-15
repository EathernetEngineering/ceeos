// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include "io/kprint.h"

void kernel_main(unsigned int magic, void *multiboot_info) {
	(void)magic, (void)multiboot_info;
	kprint("Hello, Kernel!");
	for (;;) {
		asm volatile ("hlt");
	}
	__builtin_unreachable();
}


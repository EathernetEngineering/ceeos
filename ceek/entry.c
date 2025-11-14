// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

void kernel_main(unsigned int magic, void *multiboot_info) {
	(void)magic, (void)multiboot_info;
	for (;;) {
		asm volatile ("hlt");
	}
	__builtin_unreachable();
}


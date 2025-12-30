// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <io/serial.h>
#include <arch/x86/port.h>

void serial_out(char c) {
	outb(c, 0x3F8);
}

void serial_write(const void *data, size_t size)
{
	while (size--) {
		serial_out(*(char *)data);
		data = (char *)((uintptr_t)data + 1);
	}
}


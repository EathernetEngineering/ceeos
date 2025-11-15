// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/port.h>

static inline void serial_out(char c) {
	outb(c, 0x3F8);
}

void serial_write(const char *s);


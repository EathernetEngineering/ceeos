// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <io/serial.h>

void serial_write(const char *s)
{
	while (*s) {
		serial_out(*s);
		++s;
	}
}


// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <panic.h>
#include <io/serial.h>
#include <string.h>

void panic(void) {
	static const char *const msg = "!!! PANIC";
	serial_write(msg, strlen(msg));
	fatal_spin();
}


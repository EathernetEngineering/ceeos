// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <panic.h>
#include <io/serial.h>

void panic(void) {
	serial_write("!!! PANIC");
	fatal_spin();
}


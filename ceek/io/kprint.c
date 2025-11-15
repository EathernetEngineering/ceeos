// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include "serial.h"

void kprint(const char *msg) {
	serial_write(msg);
}


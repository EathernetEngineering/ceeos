// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <io/kprint.h>

#include <kernel/panic.h>
#include <io/serial.h>

#include <lib/string.h>


void kprint(const char *msg)
{
	serial_write(msg, strlen(msg));
}


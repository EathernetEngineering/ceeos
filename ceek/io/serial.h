// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_IO_SERIAL_H_
#define CEEK_IO_SERIAL_H_

#include <stdint.h>

void serial_out(char c);
void serial_write(const void *data, size_t size);

#endif


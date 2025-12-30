// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_IO_KPRINT_H_
#define CEEK_IO_KPRINT_H_

#include <stdarg.h>

void kprint(const char *msg);
int kvprintf(const char *fmt, va_list args);
int kprintf(const char *fmt, ...);
int kvsprintf(char *restrict dst, const char *restrict fmt, va_list args);
int ksprintf(char *restrict dst, const char *restrict fmt, ...);

#endif


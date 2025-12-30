// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEE_LIBC_ITOA_H_
#define CEE_LIBC_ITOA_H_

int itoa(int i, char *buffer, int radix);
int ltoa(long i, char *buffer, int radix);
int uitoa(unsigned int i, char *buffer, int radix);
int ultoa(unsigned long i, char *buffer, int radix);

#endif


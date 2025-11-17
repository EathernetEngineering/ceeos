// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef _CEEK_LIBC_ATOI_H_
#define _CEEK_LIBC_ATOI_H_

int atoi(char *s);
long atol(char *s);
long long atoll(char *s);
long strtol(char *restrict s, char **restrict str_end, int base);
long long strtoll(char *restrict s, char **restrict str_end, int base);
unsigned long strtoul(char *restrict s, char **restrict str_end, int base);
unsigned long long strtoull(char *restrict s, char **restrict str_end, int base);
int strtimax(char *restrict s, char **restrict str_end, int base);
unsigned int strtoumax(char *restrict s, char **restrict str_end, int base);

#endif


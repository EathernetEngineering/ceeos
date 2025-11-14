// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEKLIBC_STRING_H_
#define CEEKLIBC_STRING_H_

#ifdef __cplusplus 
extern "C" {
#endif
unsigned long strlen(const char *dst);
char *strcpy(char *restrict dst, const char *restrict src);
char *strncpy(char *restrict dst, const char *restrict src, unsigned long len);
void *memcpy(void *restrict dst, const void *restrict src, unsigned long len);
void *memmove(void *dst, const void *src, unsigned long len);
void *memset(void *dst, int val, unsigned long len);
int strcmp(const char *lhs, const char *rhs);
int strncmp(const char *lhs, const char *rhs, unsigned long n);
int memcmp(const void *lhs, const void *rhs, unsigned long n);
char *strcat(char *restrict dst, const char *restrict src);
#ifdef __cplusplus 
}
#endif
#endif


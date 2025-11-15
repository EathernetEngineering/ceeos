// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <string.h>

unsigned long strlen(const char *dst) {
	unsigned long i = 0;
	while (dst[i] != '\0')
		i++;
	return i;
}

char *strcpy(char *restrict dst, const char *restrict src)
{
	if (dst == src)
		return dst;
	unsigned long i = 0;
	do {
		dst[i] = src[i];
	} while (src[i] != '\0');
	return dst;
}

char *strncpy(char *restrict dst, const char *restrict src, unsigned long len)
{
	if (dst == src)
		return dst;
	unsigned long i = 0;
	do {
		dst[i] = src[i];
	} while (src[i] != '\0' && ++i < len);
	return dst;
}

void *memcpy(void *restrict dst, const void *restrict src, unsigned long len)
{
	if (dst == src)
		return dst;
	for (unsigned long i = 0; i < len; i++) {
		*(char *)(dst + i) = *(char *)(src + i);
	}
	return dst;
}

void *memmove(void *dst, const void *src, unsigned long len)
{
	if (dst == src)
		return dst;
	for (unsigned long i = 0; i < len; i++) {
		*(char *)(dst + i) = *(char *)(src + i);
	}
	return dst;
}

void *memset(void *dst, int val, unsigned long len)
{
	for (unsigned long i = 0; i < len; i++)
		*(char *)(dst + i) = (char)val;
	return dst;
}

int strcmp(const char *lhs, const char *rhs)
{
	if (lhs == rhs)
		return 0;
	unsigned long i = 0;
	while (lhs[i] != '\0' && lhs[i] == rhs[i])
		i++;
	return lhs[i] - rhs[i];
}

int strncmp(const char *lhs, const char *rhs, unsigned long n)
{
	if (lhs == rhs)
		return 0;
	unsigned long i = 0;
	while (lhs[i] != '\0' && lhs[i] == rhs[i] && i < n)
		i++;
	return lhs[i] - rhs[i];
}

int memcmp(const void *lhs, const void *rhs, unsigned long n)
{
	if (lhs == rhs)
		return 0;
	unsigned long i = 0;
	while (*(char *)(lhs + i) == *(char *)(rhs + i) && i < n)
		i++;
	return *(char *)(lhs + i) - *(char *)(rhs + i);
}

char *strcat(char *restrict dst, const char *restrict src)
{
	return strcpy(dst + strlen(dst), src);
}


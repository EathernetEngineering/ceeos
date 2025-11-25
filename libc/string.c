// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <string.h>

unsigned long strlen(const char *s) {
	unsigned long i = 0;
	while (s[i] != '\0')
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
	} while (src[i++] != '\0');
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
		*((char *)dst + i) = *((char *)src + i);
	}
	return dst;
}

void *memmove(void *dst, const void *src, unsigned long len)
{
	if (dst == src)
		return dst;
	if (dst < src) {
		for (unsigned long i = 0; i < len; i++) {
			*((unsigned char *)dst + i) = *((unsigned char *)src + i);
		}
	} else {
		for (unsigned long i =  len - 1; i > 0; i--) {
			*((unsigned char *)dst + i) = *((unsigned char *)src + i);
		}
	}
	return dst;
}

void *memset(void *dst, char val, unsigned long len)
{
	for (unsigned long i = 0; i < len; i++)
		*((char *)dst + i) = (char)val;
	return dst;
}

int strcmp(const char *lhs, const char *rhs)
{
	if (lhs == rhs)
		return 0;
	while ((unsigned char *)lhs == (unsigned char *)rhs && *lhs != '\0') {
		lhs++;
		rhs++;
	}
	return (unsigned char *)lhs - (unsigned char *)rhs;
}

int strncmp(const char *lhs, const char *rhs, unsigned long n)
{
	if (lhs == rhs || n == 0)
		return 0;

	while (n-- > 0) {
		if (*(unsigned char *)lhs != *(unsigned char *)rhs)
			return *(unsigned char *)lhs - *(unsigned char *)rhs;
		if (*lhs == '\0')
			return 0;
		lhs++;
		rhs++;
	}
	return 0;
}

int memcmp(const void *lhs, const void *rhs, unsigned long n)
{
	const char *c1 = (const char *)lhs, *c2 = (const char *)rhs;
	if (lhs == rhs)
		return 0;
	for (unsigned long i = 0; i < n; i++) {
		if (c1[i] < c2[i])
			return -1;
		else if (c1[i] > c2[i])
			return 1;
	}
	return 0;
}

char *strcat(char *restrict dst, const char *restrict src)
{
	return strcpy(dst + strlen(dst), src);
}


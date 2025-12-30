// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEE_LIBC_BITS_UINTN_IDENTITY_H_
#define CEE_LIBC_BITS_UINTN_IDENTITY_H_

#include <lib/stdint.h>

static inline uint16_t __uint16_identity(uint16_t x)
{
	return x;
}

static inline uint32_t __uint32_identity(uint32_t x)
{
	return x;
}

static inline uint64_t __uint64_identity(uint64_t x)
{
	return x;
}

#endif


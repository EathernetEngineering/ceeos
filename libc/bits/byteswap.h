// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEE_LIBC_BITS_BYTESWAP_H_
#define CEE_LIBC_BITS_BYTESWAP_H_

#include <stdint.h>

// Swap bytes in 16-bit value.
#define __bswap_constant_16(x) \
  ((uint16_t)((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

static inline uint16_t __bswap_16 (uint16_t bsx)
{
  return __bswap_constant_16 (bsx);
}

// Swap bytes in 32-bit value.
#define __bswap_constant_32(x) \
	((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >>  8) | \
	 (((x) & 0x0000ff00u) <<  8) | (((x) & 0x000000ffu) << 24))

static inline uint32_t __bswap_32(uint32_t bsx)
{
  return __bswap_constant_32 (bsx);
}

// Swap bytes in 64-bit value.
#define __bswap_constant_64(x) \
	((((x) & 0xff00000000000000ull) >> 56) | \
	 (((x) & 0x00ff000000000000ull) >> 40) | \
	 (((x) & 0x0000ff0000000000ull) >> 24) | \
	 (((x) & 0x000000ff00000000ull) >>  8) | \
	 (((x) & 0x00000000ff000000ull) <<  8) | \
	 (((x) & 0x0000000000ff0000ull) << 24) | \
	 (((x) & 0x000000000000ff00ull) << 40) | \
	 (((x) & 0x00000000000000ffull) << 56))

static inline uint64_t __bswap_64(uint64_t bsx)
{
  return __bswap_constant_64 (bsx);
}

#endif


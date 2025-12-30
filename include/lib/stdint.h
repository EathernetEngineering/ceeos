// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_LIBC_STDINT_H_
#define CEEK_LIBC_STDINT_H_

#ifdef __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
typedef __UINT8_TYPE__ uint8_t;
#endif

#ifdef __INT16_TYPE__
typedef __INT16_TYPE__ int16_t;
typedef __UINT16_TYPE__ uint16_t;
#endif

#ifdef __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
typedef __UINT32_TYPE__ uint32_t;
#endif

#ifdef __INT64_TYPE__
typedef __INT64_TYPE__ int64_t;
typedef __UINT64_TYPE__ uint64_t;
#endif

#ifdef __INTPTR_TYPE__
typedef __INTPTR_TYPE__ intptr_t;
typedef __UINTPTR_TYPE__ uintptr_t;
#endif

#ifdef __SIZE_TYPE__
typedef __SIZE_TYPE__ size_t;
#endif

#ifdef __SSIZE_TYPE__
typedef __SSIZE_TYPE__ ssize_t;
#else
#define __SSIZE_TYPE__ \
__typeof__(_Generic(((__SIZE_TYPE__)0), \
	unsigned long long int :  (long long int)0, \
	unsigned long int      :  (long int)0, \
	unsigned int           :  (int)0, \
	unsigned short         :  (short)0, \
	unsigned char          :  (signed char)0)) \

typedef __SSIZE_TYPE__ ssize_t;
#endif

#ifdef __PTRDIFF_TYPE__
typedef __PTRDIFF_TYPE__ ptrdiff_t;
#endif

#ifdef __INTMAX_TYPE__
typedef __INTMAX_TYPE__ intmax_t;
typedef __UINTMAX_TYPE__ uintmax_t;
#endif

_Static_assert(sizeof(uint8_t) == 1,  "uint8_t must be 8 bits");
_Static_assert(sizeof(uint8_t) == 1,  "uint8_t must be 8 bits");
_Static_assert(sizeof(uint16_t) == 2, "uint16_t must be 16 bits");
_Static_assert(sizeof(uint16_t) == 2, "uint16_t must be 16 bits");
_Static_assert(sizeof(uint32_t) == 4, "uint32_t must be 32 bits");
_Static_assert(sizeof(uint32_t) == 4, "uint32_t must be 32 bits");
_Static_assert(sizeof(uint64_t) == 8, "uint64_t must be 64 bits");
_Static_assert(sizeof(uint64_t) == 8, "uint64_t must be 64 bits");
_Static_assert(sizeof(size_t) == sizeof(ssize_t),
			   "size_t and ssize_t have mismatched sizes");

#define INT8_MAX      127
#define INT8_MIN    (-INT8_MAX - 1)
#define UINT8_MAX     255
#define INT16_MAX     32767
#define INT16_MIN   (-INT16_MAX - 1)
#define UINT16_MAX    65535
#define INT32_MAX     2147483647
#define INT32_MIN   (-INT32_MAX - 1)
#define UINT32_MAX    4294967295
#define INT64_MAX     9223372036854775807l
#define INT64_MIN   (-INT64_MAX - 1)
#define UINT64_MAX    18446744073709551615ul

#endif


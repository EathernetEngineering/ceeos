//
//

#ifndef CEEK_LIBC_LIMITS_H_
#define CEEK_LIBC_LIMITS_H_

#if defined(__x86_64__) && !defined(__ILP32__)
#define __WORDSIZE 64
#else 
#define __WORDSIZE 32
#endif

#ifdef __x86_64__
#define __SYSCALL_WORDSIZE 64
#endif

#define CHAR_BIT 8

#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define UCHAR_MAX 255

#ifdef __CHAR_UNSIGNED__
#define CHAR_MIN	0
#define CHAR_MAX	UCHAR_MAX
#else
#define CHAR_MIN	SCHAR_MIN
#define CHAR_MAX	SCHAR_MAX
#endif

#define SHRT_MAX    32767
#define SHRT_MIN  (-32768)
#define USHRT_MAX   65535

#define INT_MAX     2147483647
#define INT_MIN   (-INT_MAX - 1)
#define UINT_MAX    4294967295u

#if __WORDSIZE == 64
#define LONG_MAX    9223372036854775807l
#else
#define LONG_MAX    2147483647L
#endif
#define LONG_MIN  (-LONG_MAX - 1l)
#if __WORDSIZE == 64
#define ULONG_MAX   18446744073709551615ul
#else
#define ULONG_MAX   4294967295UL
#endif
#ifdef __USE_ISOC99
#define LLONG_MAX   9223372036854775807ll
#define LLONG_MIN (-LLONG_MAX - 1ll)
#define ULLONG_MAX  18446744073709551615ull
#endif /* ISO C99 */

#define SIZE_MAX    18446744073709551615ul

#endif


// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <itoa.h>
#include <stdint.h>

int itoa(int n, char *buffer, int radix)
{
	return ltoa(n, buffer, radix);
}

int ltoa(long n, char *buffer, int radix)
{
	if (radix > 36 || radix < 2)
		return 0;

	if (n < 0) {
		if (buffer) {
			buffer[0] = '-';
			++buffer;
		}
		n *= -1;
	}
	if (n == 0) {
		buffer[0] = '0';
		return 1;
	}

	int64_t length = ((63 - __builtin_clzll((int64_t)n)) /
		(63 - __builtin_clzll((int64_t)radix)));
	for (long idx = length, digit; n > 0; idx--) {
		digit = n % radix;
		n /= radix;
		if (digit < 10)
			buffer[idx] = digit + '0';
		else
			buffer[idx] = (digit - 10) + 'A';
	}

	return (int)length + 1;
}


// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <lib/itoa.h>

static void reverse(char *buffer, int length)
{
	int start = 0;
	int end = length - 1;
	char temp;
	while (start < end) {
		temp = buffer[start];
		buffer[start] = buffer[end];
		buffer[end] = temp;
		start++;
		end--;
	}
}

int itoa(int n, char *buffer, int radix)
{
	return ltoa(n, buffer, radix);
}

int ltoa(long n, char *buffer, int radix)
{
	int i, sign;
	if (radix > 36 || radix < 2)
		return 0;

	if ((sign = n) < 0)
		n = -n;

	i = 0;
	do {
		long remainder = n % radix;
		if (remainder < 10)
			buffer[i++] = remainder + '0';
		else
			buffer[i++] = remainder + ('A' - 10);
	} while ((n /= radix) > 0);
	if (sign < 0)
		buffer[i++] = '-';
	buffer[i] = '\0';

	reverse(buffer, i);

	return i;
}

int uitoa(unsigned int n, char *buffer, int radix)
{
	return ultoa(n, buffer, radix);
}

int ultoa(unsigned long n, char *buffer, int radix)
{
	int i;
	if (radix > 36 || radix < 2)
		return 0;

	i = 0;
	do {
		unsigned long remainder = n % radix;
		if (remainder < 10)
			buffer[i++] = remainder + '0';
		else
			buffer[i++] = remainder + ('A' - 10);
	} while ((n /= radix) > 0);
	buffer[i] = '\0';

	reverse(buffer, i);

	return i;
}


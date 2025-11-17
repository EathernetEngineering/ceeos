// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <atoi.h>

#include <ctype.h>

int atoi(char *s)
{
	return (int)strtoll(s, (char**)0x0, 10);
}

long atol(char *s)
{
	return (long)strtoll(s, (char**)0x0, 10);
}

long long atoll(char *s)
{
	return strtoll(s, (char**)0x0, 10);
}

long strtol(char *restrict s, char **restrict str_end, int base)
{
	return (long)strtoll(s, str_end, base);
}

long long strtoll(char *restrict s, char **restrict str_end, int base)
{
	long long n = 0;
	int negative = 0;

	if (base > 36)
		return 0;

	while(!!isspace(*s)) {
		if (*s == 0)
			return 0;
		else if (*s == '-')
			negative = 1;
		else if (*s == '+')
			negative = 0;
		++s;
	}

	if (base == 0) {
		if (*s == '0') {
			base = 8;
			if (tolower(*(s + 1)) == 'x') {
				base = 16;
				++s;
			}
			++s;
		} else {
			base = 10;
		}
	}

	while (*s) {
		char c = toupper(*s);

		if (!isalpha(c)) {
			if ((c - 'A') > (base - 10))
				break;
		} else if (!isdigit(c)) {
			break;
		}

		n *= base;

		if (isdigit(c))
			n += c - '0';
		else
			n += (c - 'A') + 10;

		++s;
	}

	if (str_end)
		*str_end = s - 1;
	return negative ? -n : n;
}

unsigned long strtoul(char *restrict s, char **restrict str_end, int base)
{
	return (unsigned long)strtoll(s, str_end, base);
}

unsigned long long strtoull(char *restrict s, char **restrict str_end, int base)
{
	unsigned long long n = 0;

	if (base > 36)
		return 0;

	while(!!isspace(*s)) {
		if (*s == 0)
			return 0;
		++s;
	}

	if (base == 0) {
		if (*s == '0') {
			base = 8;
			if (tolower(*(s + 1)) == 'x') {
				base = 16;
				++s;
			}
			++s;
		} else {
			base = 10;
		}
	}

	while (*s) {
		char c = toupper(*s);

		if (!isalpha(c)) {
			if ((c - 'A') > (base - 10))
				break;
		} else if (!isdigit(c)) {
			break;
		}

		n *= base;

		if (isdigit(c))
			n += c - '0';
		else
			n += (c - 'A') + 10;

		++s;
	}

	if (str_end)
		*str_end = s;
	return n;
}

int strtimax(char *restrict s, char **restrict str_end, int base)
{
	return (int)strtoll(s, str_end, base);
}

unsigned int strtoumax(char *restrict s, char **restrict str_end, int base)
{
	return (unsigned int)strtoll(s, str_end, base);
}


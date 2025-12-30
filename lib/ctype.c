// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <lib/ctype.h>

int isalnum(int c)
{
	if ((c > 0x2F && c < 0x3A) ||
		(c > 0x40 && c < 0x5B) ||
		(c > 0x60 && c < 0x7B)) {
		return c;
	}
	return 0;
}

int isalpha(int c)
{
	if ((c > 0x40 && c < 0x5B) ||
		(c > 0x60 && c < 0x7B)) {
		return c;
	}
	return 0;
}

int isblank(int c)
{
	if (c == 0x09 || c == 0x20) {
		return c;
	}
	return 0;
}

int iscntrl(int c)
{
	if (c < 0x20 || c == 0x7F) {
		return c;
	}
	return 0;
}

int isdigit(int c)
{
	if ((c > 0x2f && c < 0x3A)) {
		return c;
	}
	return 0;
}

int isgraph(int c)
{
	if (c > 0x20 && c < 0x7F) {
		return c;
	}
	return 0;
}

int islower(int c)
{
	if (c > 0x60 && c < 0x7B) {
		return c;
	}
	return 0;
}

int isprint(int c)
{
	if (c > 0x1F && c < 0x7F) {
		return c;
	}
	return 0;
}

int ispunct(int c)
{
	if ((c > 0x20 && c < 0x30) ||
		(c > 0x39 && c < 0x41) ||
		(c > 0x5A && c < 0x61) ||
		(c > 0x7A && c < 0x7F)) {
		return c;
	}
	return 0;
}

int isspace(int c)
{
	if ((c > 0x08 && c < 0x0E) || c == 0x20) {
		return c;
	}
	return 0;
}

int isupper(int c)
{
	if (c > 0x40 && c < 0x5B) {
		return c;
	}
	return 0;
}

int isxdigit(int c)
{
	if ((c > 0x2f && c < 0x3A) ||
		(c > 0x40 && c < 0x47) ||
		(c > 0x60 && c < 0x67)) {
		return c;
	}
	return 0;
}

int tolower(int c)
{
	if (c > 0x40 && c < 0x5B) {
		return c + 0x20;
	}
	return c;
}

int toupper(int c)
{
	if (c > 0x60 && c < 0x7B) {
		return c - 0x20;
	}
	return c;
}


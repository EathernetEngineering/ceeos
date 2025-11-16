// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

static inline void outb(unsigned char c, unsigned short port)
{
	asm volatile ("outb %0, %1" :: "a"(c), "d"(port));
}

static inline void inb(unsigned char *c, unsigned short port)
{
	asm volatile ("inb %1, %0" : "=a"(*c) : "d"(port));
}

static inline void outs(unsigned short s, unsigned short port)
{
	asm volatile ("outs %0, %1" :: "a"(s), "d"(port));
}

static inline void ins(unsigned short *s, unsigned short port)
{
	asm volatile ("ins %1, %0" : "=a"(*s) : "d"(port));
}


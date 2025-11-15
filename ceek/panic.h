// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_PANIC_H_
#define CEEK_PANIC_H_

__attribute__((noreturn))
void panic(void);

static inline void __attribute__((__noreturn__)) fatal_spin(void)
{
	asm volatile ("cli");
	for (;;)
		asm volatile ("hlt");
}

#endif


// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_PANIC_H_
#define CEEK_PANIC_H_

#include <arch/x86/interrupts.h>

void __attribute__((__noreturn__))
	interrupt_panic(const struct isr_context *ctx);
void __attribute__((__noreturn__)) panic(void);

static inline void __attribute__((__noreturn__)) fatal_spin(void)
{
	__asm__ volatile (
		"cli\n"
		"1:\n\t"
		"hlt\n\t"
		"jmp 1b");

	__builtin_unreachable();
}

#endif


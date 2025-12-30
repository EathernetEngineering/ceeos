// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <kernel/panic.h>
#include <io/kprint.h>
#include <io/serial.h>

#include <lib/itoa.h>
#include <lib/string.h>

static inline void print_padding(int n)
{
	for (int i = 0; i < n; i++)
		serial_out(' ');
}

void panic(const char *errmsg) {
	static const char *const msg = "!!! PANIC: ";
	kprint(msg);
	kprint(errmsg);
	serial_out('\r');
	serial_out('\n');
	spin_lock();
}

void panicf(const char *fmt, ...) {
	static const char *const msg = "!!! PANIC: ";
	kprint(msg);
	va_list args;
	va_start(args, fmt);
	kvprintf(fmt, args);
	va_end(args);
	serial_out('\r');
	serial_out('\n');
	spin_lock();
}

void interrupt_panic(const struct isr_context *ctx)
{
	/*
	 * Message format:
	 *
	 * VEC: 0xFF                    EC: 0xFF
	 *
	 * RIP: 0x0000000000000000  RFLAGS: 0x0000000000000000
	 *  CS: 0x0000                  SS: 0x0000
	 * R15: 0x0000000000000000     R14: 0x0000000000000000
	 * R13: 0x0000000000000000     R12: 0x0000000000000000
	 * R11: 0x0000000000000000     R10: 0x0000000000000000
	 *  R9: 0x0000000000000000      R8: 0x0000000000000000
	 * RSI: 0x0000000000000000     RDI: 0x0000000000000000
	 * RDX: 0x0000000000000000     RCX: 0x0000000000000000
	 * RBX: 0x0000000000000000     RAX: 0x0000000000000000
	 * RBP: 0x0000000000000000
	 */
	char str[64];
	int strl;
	kprint("!!! INTERRUPT PANIC: Registers:\r\n");
	kprint("!!! VEC: ");
	strl = ltoa(ctx->interrupt_num, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("    EC: ");
	strl = ltoa(ctx->error_code, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!!\r\n!!! RIP: ");
	strl = ltoa(ctx->frame.rip, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("RFLAGS: ");
	strl = ltoa(ctx->frame.rflags, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!!  CS: ");
	strl = ltoa(ctx->frame.cs, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("    SS: ");
	strl = ltoa(ctx->frame.ss, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!! R15: ");
	strl = ltoa(ctx->r15, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("   R14: ");
	strl = ltoa(ctx->r14, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!! R13: ");
	strl = ltoa(ctx->r13, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("   R12: ");
	strl = ltoa(ctx->r12, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!! R11: ");
	strl = ltoa(ctx->r11, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("   R10: ");
	strl = ltoa(ctx->r10, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!!  R9: ");
	strl = ltoa(ctx->r9, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("    R8: ");
	strl = ltoa(ctx->r8, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!! RSI: ");
	strl = ltoa(ctx->rsi, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("   RDI: ");
	strl = ltoa(ctx->rdi, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!! RDX: ");
	strl = ltoa(ctx->rdx, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("   RCX: ");
	strl = ltoa(ctx->rcx, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!! RBX: ");
	strl = ltoa(ctx->rbx, str, 16);
	str[strl] = '\0';
	kprint(str);
	print_padding(20 - strl);
	kprint("   RAX: ");
	strl = ltoa(ctx->rax, str, 16);
	str[strl] = '\0';
	kprint(str);
	kprint("\r\n!!! RBP: ");
	strl = ltoa(ctx->rbp, str, 16);
	strcpy(str + strl, "\r\n");
	kprint(str);
	spin_lock();
}


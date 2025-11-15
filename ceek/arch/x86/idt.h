// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_IDT_H_
#define CEEK_IDT_H_

#include <stdint.h>

struct interrupt_frame {
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
};

struct isr_context {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
	uint64_t interrupt_num;
	uint64_t error_code;
	struct interrupt_frame frame;
};


struct idte {
	uint16_t offset_lower;
	uint16_t segment_selector;
	uint16_t flags;
	uint16_t offset_middle;
	uint32_t offset_upper;
	uint32_t reserved;
};

struct __attribute__((__packed__)) idt_descriptor {
	uint16_t size;
	struct idte *location;
};

typedef void(*interrupt_stub)(void);

static inline interrupt_stub idt_get_offset(const struct idte *entry)
{
	interrupt_stub offset;
	offset = (interrupt_stub)(
		((uintptr_t)entry->offset_upper << 32) &
		((uintptr_t)entry->offset_middle << 16) &
		((uintptr_t)entry->offset_lower));
	return offset;
}

static inline struct idte *idt_set_offset(struct idte *entry, interrupt_stub ptr)
{
	entry->offset_upper = (uint32_t)(((uintptr_t)ptr & ~0xFFFFFFFFull) >> 32);
	entry->offset_middle = (uint32_t)(((uintptr_t)ptr & 0xFFF0000Full) >> 16);
	entry->offset_lower = (uint32_t)(((uintptr_t)ptr & 0xFFFFull));
	return entry;
}

static inline void set_idtr(struct idt_descriptor *d)
{
	asm volatile ("lidt %0" :: "m"(*d));
}

void init_idt(void);

void isr_handler(const struct isr_context *ctx);
void irq_handler(const struct isr_context *ctx);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void irq32(void);
extern void irq33(void);
extern void irq34(void);
extern void irq35(void);
extern void irq36(void);
extern void irq37(void);
extern void irq38(void);
extern void irq39(void);
extern void irq40(void);
extern void irq41(void);
extern void irq42(void);
extern void irq43(void);
extern void irq44(void);
extern void irq45(void);
extern void irq46(void);
extern void irq47(void);
extern void isr255(void);

#endif


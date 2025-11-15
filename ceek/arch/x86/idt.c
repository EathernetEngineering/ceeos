// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/idt.h>
#include <arch/x86/gdt.h>

#include <string.h>

static struct idt_descriptor idt_desc;
static struct idte idt[64];

void init_idt(void)
{
	struct idte entry;
	idt_set_offset(&entry, isr0);
	entry.segment_selector = gdt_get_interrupt_code_segment();
	entry.flags = 0xF000; // Trap
	memcpy(&idt[0 +  0], &entry, sizeof(entry));
	idt_set_offset(&entry, isr1);
	memcpy(&idt[0 +  1], &entry, sizeof(entry));
	idt_set_offset(&entry, isr2);
	memcpy(&idt[0 +  2], &entry, sizeof(entry));
	idt_set_offset(&entry, isr3);
	memcpy(&idt[0 +  3], &entry, sizeof(entry));
	idt_set_offset(&entry, isr4);
	memcpy(&idt[0 +  4], &entry, sizeof(entry));
	idt_set_offset(&entry, isr5);
	memcpy(&idt[0 +  5], &entry, sizeof(entry));
	idt_set_offset(&entry, isr6);
	memcpy(&idt[0 +  6], &entry, sizeof(entry));
	idt_set_offset(&entry, isr7);
	memcpy(&idt[0 +  7], &entry, sizeof(entry));
	idt_set_offset(&entry, isr8);
	memcpy(&idt[0 +  8], &entry, sizeof(entry));
	idt_set_offset(&entry, isr9);
	memcpy(&idt[0 +  9], &entry, sizeof(entry));
	idt_set_offset(&entry, isr10);
	memcpy(&idt[0 + 10], &entry, sizeof(entry));
	idt_set_offset(&entry, isr11);
	memcpy(&idt[0 + 11], &entry, sizeof(entry));
	idt_set_offset(&entry, isr12);
	memcpy(&idt[0 + 12], &entry, sizeof(entry));
	idt_set_offset(&entry, isr13);
	memcpy(&idt[0 + 13], &entry, sizeof(entry));
	idt_set_offset(&entry, isr14);
	memcpy(&idt[0 + 14], &entry, sizeof(entry));
	idt_set_offset(&entry, isr15);
	memcpy(&idt[0 + 15], &entry, sizeof(entry));
	idt_set_offset(&entry, isr16);
	memcpy(&idt[0 + 16], &entry, sizeof(entry));
	idt_set_offset(&entry, isr17);
	memcpy(&idt[0 + 17], &entry, sizeof(entry));
	idt_set_offset(&entry, isr18);
	memcpy(&idt[0 + 18], &entry, sizeof(entry));
	idt_set_offset(&entry, isr19);
	memcpy(&idt[0 + 19], &entry, sizeof(entry));
	idt_set_offset(&entry, isr20);
	memcpy(&idt[0 + 20], &entry, sizeof(entry));
	idt_set_offset(&entry, isr21);
	memcpy(&idt[0 + 21], &entry, sizeof(entry));
	idt_set_offset(&entry, isr22);
	memcpy(&idt[0 + 22], &entry, sizeof(entry));
	idt_set_offset(&entry, isr23);
	memcpy(&idt[0 + 23], &entry, sizeof(entry));
	idt_set_offset(&entry, isr24);
	memcpy(&idt[0 + 24], &entry, sizeof(entry));
	idt_set_offset(&entry, isr25);
	memcpy(&idt[0 + 25], &entry, sizeof(entry));
	idt_set_offset(&entry, isr26);
	memcpy(&idt[0 + 26], &entry, sizeof(entry));
	idt_set_offset(&entry, isr27);
	memcpy(&idt[0 + 27], &entry, sizeof(entry));
	idt_set_offset(&entry, isr28);
	memcpy(&idt[0 + 28], &entry, sizeof(entry));
	idt_set_offset(&entry, isr29);
	memcpy(&idt[0 + 29], &entry, sizeof(entry));
	idt_set_offset(&entry, isr30);
	memcpy(&idt[0 + 30], &entry, sizeof(entry));
	idt_set_offset(&entry, isr31);
	memcpy(&idt[0 + 31], &entry, sizeof(entry));
	idt_set_offset(&entry, irq32);
	entry.flags = 0xE000; // Interrupt
	memcpy(&idt[0 + 32], &entry, sizeof(entry));
	idt_set_offset(&entry, irq33);
	memcpy(&idt[0 + 33], &entry, sizeof(entry));
	idt_set_offset(&entry, irq34);
	memcpy(&idt[0 + 34], &entry, sizeof(entry));
	idt_set_offset(&entry, irq35);
	memcpy(&idt[0 + 35], &entry, sizeof(entry));
	idt_set_offset(&entry, irq36);
	memcpy(&idt[0 + 36], &entry, sizeof(entry));
	idt_set_offset(&entry, irq37);
	memcpy(&idt[0 + 37], &entry, sizeof(entry));
	idt_set_offset(&entry, irq38);
	memcpy(&idt[0 + 38], &entry, sizeof(entry));
	idt_set_offset(&entry, irq39);
	memcpy(&idt[0 + 39], &entry, sizeof(entry));
	idt_set_offset(&entry, irq40);
	memcpy(&idt[0 + 40], &entry, sizeof(entry));
	idt_set_offset(&entry, irq41);
	memcpy(&idt[0 + 41], &entry, sizeof(entry));
	idt_set_offset(&entry, irq42);
	memcpy(&idt[0 + 42], &entry, sizeof(entry));
	idt_set_offset(&entry, irq43);
	memcpy(&idt[0 + 43], &entry, sizeof(entry));
	idt_set_offset(&entry, irq44);
	memcpy(&idt[0 + 44], &entry, sizeof(entry));
	idt_set_offset(&entry, irq45);
	memcpy(&idt[0 + 45], &entry, sizeof(entry));
	idt_set_offset(&entry, irq46);
	memcpy(&idt[0 + 46], &entry, sizeof(entry));
	idt_set_offset(&entry, irq47);
	memcpy(&idt[0 + 47], &entry, sizeof(entry));

	idt_desc.location = idt;
	idt_desc.size = sizeof(idt)/sizeof(idt[0]);

	set_idtr(&idt_desc);
}

void isr_handler(const struct isr_context *ctx)
{
	(void)ctx;
}

void irq_handler(const struct isr_context *ctx)
{
	(void)ctx;
}


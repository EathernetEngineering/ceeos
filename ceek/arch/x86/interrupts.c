// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/interrupts.h>
#include <arch/x86/segments.h>
#include <arch/x86/port.h>
#include <panic.h>

#include <string.h>

static struct idt_descriptor idt_desc;
static struct idte idt[64];

void init_idt(void)
{
	struct idte entry;
	memset(&entry, 0, sizeof(entry));
	memset(idt, 0, sizeof(idt)/sizeof(idt[0]));
	idt_set_offset(&entry, isr0);
	entry.segment_selector = gdt_get_interrupt_identity_code_segment();
	entry.flags = IDT_GATE_TYPE_TRAP | IDT_GATE_P;
	memcpy(&idt[ 0], &entry, sizeof(entry));
	idt_set_offset(&entry, isr1);
	memcpy(&idt[ 1], &entry, sizeof(entry));
	idt_set_offset(&entry, isr2);
	memcpy(&idt[ 2], &entry, sizeof(entry));
	idt_set_offset(&entry, isr3);
	memcpy(&idt[ 3], &entry, sizeof(entry));
	idt_set_offset(&entry, isr4);
	memcpy(&idt[ 4], &entry, sizeof(entry));
	idt_set_offset(&entry, isr5);
	memcpy(&idt[ 5], &entry, sizeof(entry));
	idt_set_offset(&entry, isr6);
	memcpy(&idt[ 6], &entry, sizeof(entry));
	idt_set_offset(&entry, isr7);
	memcpy(&idt[ 7], &entry, sizeof(entry));
	idt_set_offset(&entry, isr8);
	memcpy(&idt[ 8], &entry, sizeof(entry));
	idt_set_offset(&entry, isr9);
	memcpy(&idt[ 9], &entry, sizeof(entry));
	idt_set_offset(&entry, isr10);
	memcpy(&idt[10], &entry, sizeof(entry));
	idt_set_offset(&entry, isr11);
	memcpy(&idt[11], &entry, sizeof(entry));
	idt_set_offset(&entry, isr12);
	memcpy(&idt[12], &entry, sizeof(entry));
	idt_set_offset(&entry, isr13);
	memcpy(&idt[13], &entry, sizeof(entry));
	idt_set_offset(&entry, isr14);
	memcpy(&idt[14], &entry, sizeof(entry));
	idt_set_offset(&entry, isr15);
	memcpy(&idt[15], &entry, sizeof(entry));
	idt_set_offset(&entry, isr16);
	memcpy(&idt[16], &entry, sizeof(entry));
	idt_set_offset(&entry, isr17);
	memcpy(&idt[17], &entry, sizeof(entry));
	idt_set_offset(&entry, isr18);
	memcpy(&idt[18], &entry, sizeof(entry));
	idt_set_offset(&entry, isr19);
	memcpy(&idt[19], &entry, sizeof(entry));
	idt_set_offset(&entry, isr20);
	memcpy(&idt[20], &entry, sizeof(entry));
	idt_set_offset(&entry, isr21);
	memcpy(&idt[21], &entry, sizeof(entry));
	idt_set_offset(&entry, isr22);
	memcpy(&idt[22], &entry, sizeof(entry));
	idt_set_offset(&entry, isr23);
	memcpy(&idt[23], &entry, sizeof(entry));
	idt_set_offset(&entry, isr24);
	memcpy(&idt[24], &entry, sizeof(entry));
	idt_set_offset(&entry, isr25);
	memcpy(&idt[25], &entry, sizeof(entry));
	idt_set_offset(&entry, isr26);
	memcpy(&idt[26], &entry, sizeof(entry));
	idt_set_offset(&entry, isr27);
	memcpy(&idt[27], &entry, sizeof(entry));
	idt_set_offset(&entry, isr28);
	memcpy(&idt[28], &entry, sizeof(entry));
	idt_set_offset(&entry, isr29);
	memcpy(&idt[29], &entry, sizeof(entry));
	idt_set_offset(&entry, isr30);
	memcpy(&idt[30], &entry, sizeof(entry));
	idt_set_offset(&entry, isr31);
	memcpy(&idt[31], &entry, sizeof(entry));
	idt_set_offset(&entry, irq32);
	entry.flags = IDT_GATE_TYPE_INT | IDT_GATE_P; // Interrupt
	memcpy(&idt[32], &entry, sizeof(entry));
	idt_set_offset(&entry, irq33);
	memcpy(&idt[33], &entry, sizeof(entry));
	idt_set_offset(&entry, irq34);
	memcpy(&idt[34], &entry, sizeof(entry));
	idt_set_offset(&entry, irq35);
	memcpy(&idt[35], &entry, sizeof(entry));
	idt_set_offset(&entry, irq36);
	memcpy(&idt[36], &entry, sizeof(entry));
	idt_set_offset(&entry, irq37);
	memcpy(&idt[37], &entry, sizeof(entry));
	idt_set_offset(&entry, irq38);
	memcpy(&idt[38], &entry, sizeof(entry));
	idt_set_offset(&entry, irq39);
	memcpy(&idt[39], &entry, sizeof(entry));
	idt_set_offset(&entry, irq40);
	memcpy(&idt[40], &entry, sizeof(entry));
	idt_set_offset(&entry, irq41);
	memcpy(&idt[41], &entry, sizeof(entry));
	idt_set_offset(&entry, irq42);
	memcpy(&idt[42], &entry, sizeof(entry));
	idt_set_offset(&entry, irq43);
	memcpy(&idt[43], &entry, sizeof(entry));
	idt_set_offset(&entry, irq44);
	memcpy(&idt[44], &entry, sizeof(entry));
	idt_set_offset(&entry, irq45);
	memcpy(&idt[45], &entry, sizeof(entry));
	idt_set_offset(&entry, irq46);
	memcpy(&idt[46], &entry, sizeof(entry));
	idt_set_offset(&entry, irq47);
	memcpy(&idt[47], &entry, sizeof(entry));

	uint8_t mask = 0;
	inb(&mask, 0x21);
	outb(0x21, mask | 0x01);  // disable IRQ0

	idt_desc.size = 48 * sizeof(entry) - 1;
	idt_desc.location = idt;
	set_idtr(&idt_desc);
}

void isr_handler(const struct isr_context *ctx)
{
	(void)ctx;
	panic();
}

void irq_handler(const struct isr_context *ctx)
{
	(void)ctx;
}


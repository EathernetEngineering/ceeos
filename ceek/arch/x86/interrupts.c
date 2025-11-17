// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/interrupts.h>
#include <arch/x86/segments.h>
#include <arch/x86/port.h>
#include <panic.h>
#include <io/kprint.h>

#include <string.h>

static struct idt_descriptor idt_desc;
static struct idte idt[UINT8_MAX];

void idt_init(void)
{
	struct idte entry;
	memset(&entry, 0, sizeof(entry));
	memset(idt, 0, sizeof(idt)/sizeof(idt[0]));
	idte_set_offset(&entry, isr0);
	entry.segment_selector = gdt_get_interrupt_identity_code_segment();
	entry.flags = IDT_GATE_TYPE_TRAP | IDT_GATE_P;
	memcpy(&idt[ 0], &entry, sizeof(entry));
	idte_set_offset(&entry, isr1);
	memcpy(&idt[ 1], &entry, sizeof(entry));
	idte_set_offset(&entry, isr2);
	memcpy(&idt[ 2], &entry, sizeof(entry));
	idte_set_offset(&entry, isr3);
	memcpy(&idt[ 3], &entry, sizeof(entry));
	idte_set_offset(&entry, isr4);
	memcpy(&idt[ 4], &entry, sizeof(entry));
	idte_set_offset(&entry, isr5);
	memcpy(&idt[ 5], &entry, sizeof(entry));
	idte_set_offset(&entry, isr6);
	memcpy(&idt[ 6], &entry, sizeof(entry));
	idte_set_offset(&entry, isr7);
	memcpy(&idt[ 7], &entry, sizeof(entry));
	idte_set_offset(&entry, isr8);
	memcpy(&idt[ 8], &entry, sizeof(entry));
	idte_set_offset(&entry, isr9);
	memcpy(&idt[ 9], &entry, sizeof(entry));
	idte_set_offset(&entry, isr10);
	memcpy(&idt[10], &entry, sizeof(entry));
	idte_set_offset(&entry, isr11);
	memcpy(&idt[11], &entry, sizeof(entry));
	idte_set_offset(&entry, isr12);
	memcpy(&idt[12], &entry, sizeof(entry));
	idte_set_offset(&entry, isr13);
	memcpy(&idt[13], &entry, sizeof(entry));
	idte_set_offset(&entry, isr14);
	memcpy(&idt[14], &entry, sizeof(entry));
	idte_set_offset(&entry, isr15);
	memcpy(&idt[15], &entry, sizeof(entry));
	idte_set_offset(&entry, isr16);
	memcpy(&idt[16], &entry, sizeof(entry));
	idte_set_offset(&entry, isr17);
	memcpy(&idt[17], &entry, sizeof(entry));
	idte_set_offset(&entry, isr18);
	memcpy(&idt[18], &entry, sizeof(entry));
	idte_set_offset(&entry, isr19);
	memcpy(&idt[19], &entry, sizeof(entry));
	idte_set_offset(&entry, isr20);
	memcpy(&idt[20], &entry, sizeof(entry));
	idte_set_offset(&entry, isr21);
	memcpy(&idt[21], &entry, sizeof(entry));
	idte_set_offset(&entry, isr22);
	memcpy(&idt[22], &entry, sizeof(entry));
	idte_set_offset(&entry, isr23);
	memcpy(&idt[23], &entry, sizeof(entry));
	idte_set_offset(&entry, isr24);
	memcpy(&idt[24], &entry, sizeof(entry));
	idte_set_offset(&entry, isr25);
	memcpy(&idt[25], &entry, sizeof(entry));
	idte_set_offset(&entry, isr26);
	memcpy(&idt[26], &entry, sizeof(entry));
	idte_set_offset(&entry, isr27);
	memcpy(&idt[27], &entry, sizeof(entry));
	idte_set_offset(&entry, isr28);
	memcpy(&idt[28], &entry, sizeof(entry));
	idte_set_offset(&entry, isr29);
	memcpy(&idt[29], &entry, sizeof(entry));
	idte_set_offset(&entry, isr30);
	memcpy(&idt[30], &entry, sizeof(entry));
	idte_set_offset(&entry, isr31);
	memcpy(&idt[31], &entry, sizeof(entry));
	idte_set_offset(&entry, irq32);
	entry.flags = IDT_GATE_TYPE_INT | IDT_GATE_P; // Interrupt
	memcpy(&idt[32], &entry, sizeof(entry));
	idte_set_offset(&entry, irq33);
	memcpy(&idt[33], &entry, sizeof(entry));
	idte_set_offset(&entry, irq34);
	memcpy(&idt[34], &entry, sizeof(entry));
	idte_set_offset(&entry, irq35);
	memcpy(&idt[35], &entry, sizeof(entry));
	idte_set_offset(&entry, irq36);
	memcpy(&idt[36], &entry, sizeof(entry));
	idte_set_offset(&entry, irq37);
	memcpy(&idt[37], &entry, sizeof(entry));
	idte_set_offset(&entry, irq38);
	memcpy(&idt[38], &entry, sizeof(entry));
	idte_set_offset(&entry, irq39);
	memcpy(&idt[39], &entry, sizeof(entry));
	idte_set_offset(&entry, irq40);
	memcpy(&idt[40], &entry, sizeof(entry));
	idte_set_offset(&entry, irq41);
	memcpy(&idt[41], &entry, sizeof(entry));
	idte_set_offset(&entry, irq42);
	memcpy(&idt[42], &entry, sizeof(entry));
	idte_set_offset(&entry, irq43);
	memcpy(&idt[43], &entry, sizeof(entry));
	idte_set_offset(&entry, irq44);
	memcpy(&idt[44], &entry, sizeof(entry));
	idte_set_offset(&entry, irq45);
	memcpy(&idt[45], &entry, sizeof(entry));
	idte_set_offset(&entry, irq46);
	memcpy(&idt[46], &entry, sizeof(entry));
	idte_set_offset(&entry, irq47);
	memcpy(&idt[47], &entry, sizeof(entry));

	idte_set_offset(&entry, isr255);
	for (int32_t i = 48; i < UINT8_MAX; i++) {
		memcpy(&idt[i], &entry, sizeof(entry));
	}

	uint8_t mask = 0;
	inb(&mask, 0x21);
	outb(0x21, mask | 0x01);  // disable IRQ0

	idt_desc.size = 48 * sizeof(entry) - 1;
	idt_desc.location = idt;
	idtr_set(&idt_desc);
}

void isr_handler(const struct isr_context *ctx)
{
	switch (ctx->interrupt_num) {
		case EXCEPTION_DE:
			kprint("Divide by zero fault caught.\r\n");
			break;

		case EXCEPTION_DB:
			kprint("Debug trap caught.\r\n");
			break;

		case EXCEPTION_BP:
			kprint("Divide by zero fault caught.\r\n");
			break;

		case EXCEPTION_OF:
			kprint("Divide by zero fault caught.\r\n");
			break;

		case EXCEPTION_BR:
			kprint("Bounds range fault caught.\r\n");
			break;

		case EXCEPTION_UD:
			kprint("Invalid opcode fault caught.\r\n");
			break;

		case EXCEPTION_NM:
			kprint("Device not availible fault caught.\r\n");
			break;

		case EXCEPTION_DF:
			kprint("!!! Double fault\r\n");
			panic();
			break;

		case EXCEPTION_TS:
			kprint("Invalid TSS fault caught.\r\n");
			break;

		case EXCEPTION_NP:
			kprint("Segment not present fault caught.\r\n");
			break;

		case EXCEPTION_SS:
			kprint("Stack-segemtnt fault caught.\r\n");
			break;

		case EXCEPTION_GP:
			kprint("General protection fault caught.\r\n");
			break;

		case EXCEPTION_PF:
			kprint("Page fault caught.\r\n");
			break;

		case EXCEPTION_MF:
			kprint("x87 floating point exception caught.\r\n");
			break;

		case EXCEPTION_AC:
			kprint("Alignment check fault caught.\r\n");
			break;

		case EXCEPTION_MC:
			kprint("!!! Machine check.\r\n");
			panic();
			break;

		case EXCEPTION_XM:
			kprint("SIMD floating point fault caught.\r\n");
			break;

		case EXCEPTION_VE:
			kprint("Virtualization exception caught.\r\n");
			break;

		case EXCEPTION_CP:
			kprint("Control protection ecxception caught.\r\n");
			break;

		case EXCEPTION_HV:
			kprint("Hypervisor injection exception caught.\r\n");
			break;

		case EXCEPTION_VC:
			kprint("VMM communication exception caught.\r\n");
			break;

		case EXCEPTION_SX:
			kprint("Security exception caught.\r\n");
			break;

		default:
			kprint("Unknown isr called");
	}
}

void irq_handler(const struct isr_context *ctx)
{
	kprint("Caught IRQ");
	(void)ctx;
}


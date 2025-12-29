// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <arch/x86/interrupts.h>
#include <arch/x86/segments.h>
#include <arch/x86/port.h>
#include <io/kprint.h>
#include <panic.h>
#include <vmm.h>

#include <itoa.h>
#include <string.h>

static struct idt_descriptor g_idt_desc;
static struct idte g_idt[UINT8_MAX];

void __init_idt(void)
{
	struct idte entry;
	memset(&entry, 0, sizeof(entry));
	memset(g_idt, 0, sizeof(g_idt)/sizeof(g_idt[0]));
	idte_set_offset(&entry, isr0);
	entry.segment_selector = gdt_get_interrupt_identity_code_segment();
	entry.flags = IDT_GATE_TYPE_TRAP | IDT_GATE_P;
	memcpy(&g_idt[ 0], &entry, sizeof(entry));
	idte_set_offset(&entry, isr1);
	memcpy(&g_idt[ 1], &entry, sizeof(entry));
	idte_set_offset(&entry, isr2);
	memcpy(&g_idt[ 2], &entry, sizeof(entry));
	idte_set_offset(&entry, isr3);
	memcpy(&g_idt[ 3], &entry, sizeof(entry));
	idte_set_offset(&entry, isr4);
	memcpy(&g_idt[ 4], &entry, sizeof(entry));
	idte_set_offset(&entry, isr5);
	memcpy(&g_idt[ 5], &entry, sizeof(entry));
	idte_set_offset(&entry, isr6);
	memcpy(&g_idt[ 6], &entry, sizeof(entry));
	idte_set_offset(&entry, isr7);
	memcpy(&g_idt[ 7], &entry, sizeof(entry));
	idte_set_offset(&entry, isr8);
	memcpy(&g_idt[ 8], &entry, sizeof(entry));
	idte_set_offset(&entry, isr9);
	memcpy(&g_idt[ 9], &entry, sizeof(entry));
	idte_set_offset(&entry, isr10);
	memcpy(&g_idt[10], &entry, sizeof(entry));
	idte_set_offset(&entry, isr11);
	memcpy(&g_idt[11], &entry, sizeof(entry));
	idte_set_offset(&entry, isr12);
	memcpy(&g_idt[12], &entry, sizeof(entry));
	idte_set_offset(&entry, isr13);
	memcpy(&g_idt[13], &entry, sizeof(entry));
	idte_set_offset(&entry, isr14);
	memcpy(&g_idt[14], &entry, sizeof(entry));
	idte_set_offset(&entry, isr15);
	memcpy(&g_idt[15], &entry, sizeof(entry));
	idte_set_offset(&entry, isr16);
	memcpy(&g_idt[16], &entry, sizeof(entry));
	idte_set_offset(&entry, isr17);
	memcpy(&g_idt[17], &entry, sizeof(entry));
	idte_set_offset(&entry, isr18);
	memcpy(&g_idt[18], &entry, sizeof(entry));
	idte_set_offset(&entry, isr19);
	memcpy(&g_idt[19], &entry, sizeof(entry));
	idte_set_offset(&entry, isr20);
	memcpy(&g_idt[20], &entry, sizeof(entry));
	idte_set_offset(&entry, isr21);
	memcpy(&g_idt[21], &entry, sizeof(entry));
	idte_set_offset(&entry, isr22);
	memcpy(&g_idt[22], &entry, sizeof(entry));
	idte_set_offset(&entry, isr23);
	memcpy(&g_idt[23], &entry, sizeof(entry));
	idte_set_offset(&entry, isr24);
	memcpy(&g_idt[24], &entry, sizeof(entry));
	idte_set_offset(&entry, isr25);
	memcpy(&g_idt[25], &entry, sizeof(entry));
	idte_set_offset(&entry, isr26);
	memcpy(&g_idt[26], &entry, sizeof(entry));
	idte_set_offset(&entry, isr27);
	memcpy(&g_idt[27], &entry, sizeof(entry));
	idte_set_offset(&entry, isr28);
	memcpy(&g_idt[28], &entry, sizeof(entry));
	idte_set_offset(&entry, isr29);
	memcpy(&g_idt[29], &entry, sizeof(entry));
	idte_set_offset(&entry, isr30);
	memcpy(&g_idt[30], &entry, sizeof(entry));
	idte_set_offset(&entry, isr31);
	memcpy(&g_idt[31], &entry, sizeof(entry));
	idte_set_offset(&entry, irq32);
	entry.flags = IDT_GATE_TYPE_INT | IDT_GATE_P; // Interrupt
	memcpy(&g_idt[32], &entry, sizeof(entry));
	idte_set_offset(&entry, irq33);
	memcpy(&g_idt[33], &entry, sizeof(entry));
	idte_set_offset(&entry, irq34);
	memcpy(&g_idt[34], &entry, sizeof(entry));
	idte_set_offset(&entry, irq35);
	memcpy(&g_idt[35], &entry, sizeof(entry));
	idte_set_offset(&entry, irq36);
	memcpy(&g_idt[36], &entry, sizeof(entry));
	idte_set_offset(&entry, irq37);
	memcpy(&g_idt[37], &entry, sizeof(entry));
	idte_set_offset(&entry, irq38);
	memcpy(&g_idt[38], &entry, sizeof(entry));
	idte_set_offset(&entry, irq39);
	memcpy(&g_idt[39], &entry, sizeof(entry));
	idte_set_offset(&entry, irq40);
	memcpy(&g_idt[40], &entry, sizeof(entry));
	idte_set_offset(&entry, irq41);
	memcpy(&g_idt[41], &entry, sizeof(entry));
	idte_set_offset(&entry, irq42);
	memcpy(&g_idt[42], &entry, sizeof(entry));
	idte_set_offset(&entry, irq43);
	memcpy(&g_idt[43], &entry, sizeof(entry));
	idte_set_offset(&entry, irq44);
	memcpy(&g_idt[44], &entry, sizeof(entry));
	idte_set_offset(&entry, irq45);
	memcpy(&g_idt[45], &entry, sizeof(entry));
	idte_set_offset(&entry, irq46);
	memcpy(&g_idt[46], &entry, sizeof(entry));
	idte_set_offset(&entry, irq47);
	memcpy(&g_idt[47], &entry, sizeof(entry));

	idte_set_offset(&entry, isr255);
	for (int32_t i = 48; i < UINT8_MAX; i++) {
		memcpy(&g_idt[i], &entry, sizeof(entry));
	}

	uint8_t mask = 0;
	inb(&mask, 0x21);
	outb(0x21, mask | 0x01);  // disable IRQ0

	g_idt_desc.size = 48 * sizeof(entry) - 1;
	g_idt_desc.location = g_idt;
	idtr_set(&g_idt_desc);
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
			kprint("Breakpoint caught.\r\n");
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
			interrupt_panic(ctx);
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
			kprint("!!! General protection fault caught.\r\n");
			interrupt_panic(ctx);
			break;

		case EXCEPTION_PF:
			kprint("Page fault caught.\r\n");
			vmm_handle_pf(ctx);
			break;

		case EXCEPTION_MF:
			kprint("x87 floating point exception caught.\r\n");
			break;

		case EXCEPTION_AC:
			kprint("Alignment check fault caught.\r\n");
			break;

		case EXCEPTION_MC:
			kprint("!!! Machine check.\r\n");
			interrupt_panic(ctx);
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
			kprint("!!! Unknown isr called.\r\n");
			interrupt_panic(ctx);
	}
}

void irq_handler(const struct isr_context *ctx)
{
	char str[64];
	int strl = itoa(ctx->error_code, str, 10);
	strcpy(str + strl, "\r\n");
	kprint("Caught IRQ ");
	kprint(str);
}


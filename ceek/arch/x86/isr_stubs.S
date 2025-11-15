// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.extern isr_handler
.extern irq_handler

.global isr_common_stub
.type isr_common_stub,@function

.global irq_common_stub
.type irq_common_stub,@function

.macro ISR_STUB_ERR vec
.global isr\vec
.type isr\vec,@function
isr\vec:
	pushq $\vec
	jmp isr_common_stub
.size isr\vec, .-isr\vec
.endm

.macro ISR_STUB vec
.global isr\vec
.type isr\vec,@function
isr\vec:
	subq $16, %rsp
	movq $0, 8(%rsp)
	movq $\vec, (%rsp)
	jmp isr_common_stub
.size isr\vec, .-isr\vec
.endm

.macro IRQ_STUB vec ec
.global irq\vec
.type irq\vec,@function
irq\vec:
	subq $16, %rsp
	movq $\ec, 8(%rsp)
	movq $\vec, (%rsp)
	jmp irq_common_stub
.size irq\vec, .-irq\vec
.endm

.section .text
isr_common_stub:
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rbp
	pushq %rdi
	pushq %rsi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp, %rdi
	call isr_handler
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rsi
	popq %rdi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	addq $16, %rsp
	iretq

irq_common_stub:
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rbp
	pushq %rdi
	pushq %rsi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp, %rdi
	call irq_handler
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rsi
	popq %rdi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	addq $16, %rsp
	iretq

ISR_STUB     0        // #DE
ISR_STUB     1        // #DB
ISR_STUB     2        // NMI
ISR_STUB     3        // #BP
ISR_STUB     4        // #OF
ISR_STUB     5        // #BR
ISR_STUB     6        // #UD
ISR_STUB     7        // #NM
ISR_STUB_ERR 8        // #DF
ISR_STUB     9        // (reserved)
ISR_STUB_ERR 10       // #TS
ISR_STUB_ERR 11       // #NP
ISR_STUB_ERR 12       // #SS
ISR_STUB_ERR 13       // #GP
ISR_STUB_ERR 14       // #DPF
ISR_STUB     15       // (reserved)
ISR_STUB     16       // #MF
ISR_STUB_ERR 17       // #AC
ISR_STUB     18       // #MC
ISR_STUB     19       // #XM
ISR_STUB     20       // #VE
ISR_STUB_ERR 21       // #CP
ISR_STUB     22       // (reserved)
ISR_STUB     23       // (reserved)
ISR_STUB     24       // (reserved)
ISR_STUB     25       // (reserved)
ISR_STUB     26       // (reserved)
ISR_STUB     27       // (reserved)
ISR_STUB     28       // (reserved)
ISR_STUB     29       // (reserved)
ISR_STUB     30       // (reserved)
ISR_STUB     31       // (reserved)
IRQ_STUB     32 0     // IRQ0
IRQ_STUB     33 1     // IRQ1
IRQ_STUB     34 2     // IRQ2
IRQ_STUB     35 3     // IRQ3
IRQ_STUB     36 4     // IRQ4
IRQ_STUB     37 5     // IRQ5
IRQ_STUB     38 6     // IRQ6
IRQ_STUB     39 7     // IRQ7
IRQ_STUB     40 8     // IRQ8
IRQ_STUB     41 9     // IRQ9
IRQ_STUB     42 10    // IRQ10
IRQ_STUB     43 11    // IRQ11
IRQ_STUB     44 12    // IRQ12
IRQ_STUB     45 13    // IRQ13
IRQ_STUB     46 14    // IRQ14
IRQ_STUB     47 15    // IRQ15
ISR_STUB     255      // #svr


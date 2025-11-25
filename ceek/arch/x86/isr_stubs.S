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
	pushq $0
	pushq $\vec
	jmp isr_common_stub
.size isr\vec, .-isr\vec
.endm

.macro IRQ_STUB vec ec
.global irq\vec
.type irq\vec,@function
irq\vec:
	pushq $\ec
	pushq $\vec
	jmp irq_common_stub
.size irq\vec, .-irq\vec
.endm

.section .text
.type isr_common_stub,@function
isr_common_stub:
.cfi_startproc
.cfi_signal_frame
.cfi_def_cfa %rsp,0x28
.cfi_return_column %rip
.cfi_offset %rip,-0x18

	pushq %rbp
.cfi_adjust_cfa_offset 8
.cfi_offset %rbp,-0x30
	pushq %rax
.cfi_adjust_cfa_offset 8
.cfi_offset %rax,-0x38
	pushq %rbx
.cfi_adjust_cfa_offset 8
.cfi_offset %rbx,-0x40
	pushq %rcx
.cfi_adjust_cfa_offset 8
.cfi_offset %rcx,-0x48
	pushq %rdx
.cfi_adjust_cfa_offset 8
.cfi_offset %rdx,-0x50
	pushq %rdi
.cfi_adjust_cfa_offset 8
.cfi_offset %rdi,-0x58
	pushq %rsi
.cfi_adjust_cfa_offset 8
.cfi_offset %rsi,-0x60
	pushq %r8
.cfi_adjust_cfa_offset 8
.cfi_offset %r8,-0x68
	pushq %r9
.cfi_adjust_cfa_offset 8
.cfi_offset %r9,-0x70
	pushq %r10
.cfi_adjust_cfa_offset 8
.cfi_offset %r10,-0x78
	pushq %r11
.cfi_adjust_cfa_offset 8
.cfi_offset %r11,-0x80
	pushq %r12
.cfi_adjust_cfa_offset 8
.cfi_offset %r12,-0x88
	pushq %r13
.cfi_adjust_cfa_offset 8
.cfi_offset %r13,-0x90
	pushq %r14
.cfi_adjust_cfa_offset 8
.cfi_offset %r14,-0x98
	pushq %r15
.cfi_adjust_cfa_offset 8
.cfi_offset %r15,-0xA0

	movq %rsp, %rdi
	call isr_handler

	popq %r15
.cfi_adjust_cfa_offset -8
.cfi_restore %r15
	popq %r14
.cfi_adjust_cfa_offset -8
.cfi_restore %r14
	popq %r13
.cfi_adjust_cfa_offset -8
.cfi_restore %r13
	popq %r12
.cfi_adjust_cfa_offset -8
.cfi_restore %r12
	popq %r11
.cfi_adjust_cfa_offset -8
.cfi_restore %r11
	popq %r10
.cfi_adjust_cfa_offset -8
.cfi_restore %r10
	popq %r9
.cfi_adjust_cfa_offset -8
.cfi_restore %r9
	popq %r8
.cfi_adjust_cfa_offset -8
.cfi_restore %r8
	popq %rsi
.cfi_adjust_cfa_offset -8
.cfi_restore %rsi
	popq %rdi
.cfi_adjust_cfa_offset -8
.cfi_restore %rdi
	popq %rdx
.cfi_adjust_cfa_offset -8
.cfi_restore %rdx
	popq %rcx
.cfi_adjust_cfa_offset -8
.cfi_restore %rcx
	popq %rbx
.cfi_adjust_cfa_offset -8
.cfi_restore %rbx
	popq %rax
.cfi_adjust_cfa_offset -8
.cfi_restore %rax
	popq %rbp
.cfi_adjust_cfa_offset -8
.cfi_restore %rbp
.cfi_def_cfa %rsp,40
	addq $16, %rsp
	iretq
.cfi_endproc
.size isr_common_stub,.-isr_common_stub

.type irq_common_stub,@function
irq_common_stub:
.cfi_startproc
.cfi_signal_frame
.cfi_def_cfa %rsp,0x28
.cfi_return_column %rip
.cfi_offset %rip,-0x18

	pushq %rbp
.cfi_adjust_cfa_offset 8
.cfi_offset %rbp,-0x30
	pushq %rax
.cfi_adjust_cfa_offset 8
.cfi_offset %rax,-0x38
	pushq %rbx
.cfi_adjust_cfa_offset 8
.cfi_offset %rbx,-0x40
	pushq %rcx
.cfi_adjust_cfa_offset 8
.cfi_offset %rcx,-0x48
	pushq %rdx
.cfi_adjust_cfa_offset 8
.cfi_offset %rdx,-0x50
	pushq %rdi
.cfi_adjust_cfa_offset 8
.cfi_offset %rdi,-0x58
	pushq %rsi
.cfi_adjust_cfa_offset 8
.cfi_offset %rsi,-0x60
	pushq %r8
.cfi_adjust_cfa_offset 8
.cfi_offset %r8,-0x68
	pushq %r9
.cfi_adjust_cfa_offset 8
.cfi_offset %r9,-0x70
	pushq %r10
.cfi_adjust_cfa_offset 8
.cfi_offset %r10,-0x78
	pushq %r11
.cfi_adjust_cfa_offset 8
.cfi_offset %r11,-0x80
	pushq %r12
.cfi_adjust_cfa_offset 8
.cfi_offset %r12,-0x88
	pushq %r13
.cfi_adjust_cfa_offset 8
.cfi_offset %r13,-0x90
	pushq %r14
.cfi_adjust_cfa_offset 8
.cfi_offset %r14,-0x98
	pushq %r15
.cfi_adjust_cfa_offset 8
.cfi_offset %r15,-0xA0

	leaq 0xA0(%rsp), %rdi
	call irq_handler

	popq %r15
.cfi_adjust_cfa_offset -8
.cfi_restore %r15
	popq %r14
.cfi_adjust_cfa_offset -8
.cfi_restore %r14
	popq %r13
.cfi_adjust_cfa_offset -8
.cfi_restore %r13
	popq %r12
.cfi_adjust_cfa_offset -8
.cfi_restore %r12
	popq %r11
.cfi_adjust_cfa_offset -8
.cfi_restore %r11
	popq %r10
.cfi_adjust_cfa_offset -8
.cfi_restore %r10
	popq %r9
.cfi_adjust_cfa_offset -8
.cfi_restore %r9
	popq %r8
.cfi_adjust_cfa_offset -8
.cfi_restore %r8
	popq %rsi
.cfi_adjust_cfa_offset -8
.cfi_restore %rsi
	popq %rdi
.cfi_adjust_cfa_offset -8
.cfi_restore %rdi
	popq %rdx
.cfi_adjust_cfa_offset -8
.cfi_restore %rdx
	popq %rcx
.cfi_adjust_cfa_offset -8
.cfi_restore %rcx
	popq %rbx
.cfi_adjust_cfa_offset -8
.cfi_restore %rbx
	popq %rax
.cfi_adjust_cfa_offset -8
.cfi_restore %rax
	popq %rbp
.cfi_adjust_cfa_offset -8
.cfi_restore %rbp
.cfi_def_cfa %rsp,40
	addq $16, %rsp
	iretq
.cfi_endproc
.size irq_common_stub,.-irq_common_stub

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


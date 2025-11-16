// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.section .text
.global set_gdtr
.type set_gdtr,@function
set_gdtr:
	push %rbp
	movq %rsp, %rbp
	lgdt (%rdi)
	movq %rdx, %rax
	movw %ax, %ss
	movq %rcx, %rax
	movw %ax, %ds
	movq %r8, %rax
	movw %ax, %es
	pushq %rsi
	leaq 1f(%rip), %rax
	pushq %rax
	lretq
1:
	pop %rbp
	ret
.size set_gdtr,.-set_gdtr

.section .bss
gdt_far_jump_selector:
.quad 0
.short 0


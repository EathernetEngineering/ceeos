// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.section .text
.global gdtr_set
.type gdtr_set,@function
gdtr_set:
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
.size gdtr_set,.-gdtr_set


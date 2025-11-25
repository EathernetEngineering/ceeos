// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.section .data
.align 8
.global gdtr_jmp_addr
.type gdtr_jmp_addr,@object
gdtr_jmp_addr:
.quad 1f
.word 0

.section .text
.align 8
.global gdtr_set
.type gdtr_set,@function
gdtr_set:
	pushq %rbp
	movq %rsp, %rbp
	pushq %rax
	lgdt (%rdi)
	movq %rdx, %rax
	movw %ax, %ss
	movq %rcx, %rax
	movw %ax, %ds
	movq %r8, %rax
	movw %ax, %es
	movq $gdtr_jmp_addr, %rax
	movw %si, 8(%rax)
rex64	ljmp *(%rax)
1:
	popq %rax
	popq %rbp
	ret
.size gdtr_set,.-gdtr_set


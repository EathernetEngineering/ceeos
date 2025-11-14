// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.section .text
.code64
.extern kernel_main

.code32
.global start, _start
.type start,@function
.type _start,@function
start:
_start:
	jmp multiboot_entry
.align 8
.global multiboot_entry
.type multiboot_entry,@function
multiboot_entry:
	.cfi_startproc
	// Set up stack
	movl $stack_top, %esp
	.cfi_adjust_cfa_offset
	movl %esp, %ebp
	.cfi_def_cfa_register rbp
	.cfi_undefined rip
	// Clear %eflags
	pushl $0
	popf

	// Store these for the jump to the kernel
	movl %eax, mb_magic
	movl %ebx, mb_info_addr

	cli

	// Ensure paging is disabled to change the address in a known state
	movl %cr0, %eax
	andl $0x7FFFFFFF, %eax // ~(1 << 31) = 0x7FFFFFFF = PG bit
	movl %eax, %cr0

	// Load a known GDT
	lgdt long_mode_transition_gdt_descriptor
	mov $LONG_MODE_TRANSITION_GDT_DATA32_INDEX, %ax
	mov %ax, %ss
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	ljmp $LONG_MODE_TRANSITION_GDT_CODE32_INDEX, $1f
1:
	// Zero the PML4, PDPT, and PD (8 * 512 * 3 = 0x3000 entries)
	movl $PML4, %edi
	xorl %ecx, %ecx
	xorl %eax, %eax
2:
	movl %eax, (%edi)
	incl %edi
	cmpl $0x3000, %ecx
	je 2b

	// Set up the Page Map Level 4
	movl $PDPT, %eax
	orl $0x03, %eax // 0x03 = Present | RW
	movl %eax, PML4
	movl $0, PML4+4

	// Set up the Page Directory Pointer Table
	movl $PD, %eax
	orl $0x03, %eax // 0x03 = Present | RW
	movl %eax, PDPT
	movl $0, PDPT+4

	// Set up the Page Directory
	movl $512, %ecx
	xorl %ebx, %ebx
3:
	movl %ebx, %eax
	shll $21, %eax
	orl $0x83, %eax // 0x03 = Page Size | Present | RW
	movl %eax, PD(,%ebx,8)
	movl $0, PD+4(,%ebx,8)
	incl %ebx
	decl %ecx
	test %ecx, %ecx
	jnz 3b

	// Enable Physical Address Extension
	movl %cr4, %eax
	orl $0x20, %eax // 1 << 5 = 0x20 = PAE bit
	movl %eax, %cr4

	// Set the PML4 address
	movl $PML4, %eax
	movl %eax, %cr3

	// Set Long Mode Enable
	movl $0xC0000080, %ecx
	rdmsr
	orl $0x100, %eax // 1 << 8 = 0x100 = LME bit
	wrmsr

	// Now enable paging
	movl %cr0, %eax
	orl $0x80000000, %eax // 1 << 31 = 0x80000000 = PG bit
	movl %eax, %cr0

	mov $LONG_MODE_TRANSITION_GDT_DATA64_INDEX, %ax
	mov %ax, %ss
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	ljmp $LONG_MODE_TRANSITION_GDT_CODE64_INDEX, $.Lentry64


.align 8
.code64
.Lentry64:
	movl (mb_magic), %eax
	movl %eax, %edi
	movl (mb_info_addr), %eax
	movl %eax, %esi
	call kernel_main
1:
	hlt
	jmp 1b
	.cfi_endproc

.section .bss
.align 16
.global stack_bottom
stack_bottom:
	.skip 16384
.global stack_top
stack_top:
.global mb_magic
mb_magic:
	.long 0
.global mb_info_addr
mb_info_addr:
	.long 0
.align 4096
.global PML4
PML4:
.space 4096, 0
.global PDPT
PDPT:
.skip 4096, 0
.global PD
PD: .space 4096, 0


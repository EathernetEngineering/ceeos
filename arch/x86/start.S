// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.extern kernel_main
.extern long_mode_transition_gdt_descriptor
.extern LONG_MODE_TRANSITION_GDT_NULL_INDEX
.extern LONG_MODE_TRANSITION_GDT_CODE32_INDEX
.extern LONG_MODE_TRANSITION_GDT_DATA32_INDEX
.extern LONG_MODE_TRANSITION_GDT_CODE64_INDEX
.extern LONG_MODE_TRANSITION_GDT_DATA64_INDEX

.section .boot.text,"ax"
.code32
.global start, _start
.type start,@function
.type _start,@function
start:
_start:
	jmp	multiboot_entry
.size start,.-start
.size _start,.-_start

.align 8
.global multiboot_entry
.type multiboot_entry,@function
multiboot_entry:
	.cfi_startproc
	.cfi_undefined rip
	// Set up stack
	movl	$stack_top,	%esp
	movl	%esp,	%ebp
	.cfi_def_cfa_register %rbp
	// Clear %eflags
	pushl	$0
	popf

	// Store these for the jump to the kernel
	movl %eax,	mb_magic
	movl %ebx,	mb_info_addr

	cli

	// Ensure paging is disabled to change the address in a known state
	movl %cr0,	%eax
	andl $0x7FFFFFFF,	%eax // ~(1 << 31) = 0x7FFFFFFF = PG bit
	movl %eax,	%cr0

	// Load a known GDT
	lgdt	long_mode_transition_gdt_descriptor
	movw	$LONG_MODE_TRANSITION_GDT_DATA32_INDEX,	%ax
	movw	%ax,	%ss
	movw	%ax,	%ds
	movw	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs
	ljmp	$LONG_MODE_TRANSITION_GDT_CODE32_INDEX,	$1f
1:
	// Zero the pml4, PDPT, and PD
	// length / register size = entry count
	movl	$boot_pml4,	%edi
	movl	$BOOT_PT_LENGTH,	%ecx
	shrl	$2,	%ecx // Divide by sizeof(int)
	xorl	%eax,	%eax
	cld
	rep	stosl

	// Set up the Page Map Level 4
	movl	$boot_pdpt_low,  %eax
	orl	$0x03,	%eax                   // 0x03 = Present | RW
	movl	%eax,	boot_pml4      + 0x000 // pml4[0] low 32 bits
	movl	$0,	boot_pml4      + 0x004 // pml4[0] high 32 bits

	movl	$boot_pdpt_high,	%eax
	orl	$0x03,	%eax                   // 0x03 = Present | RW
	movl	%eax,	boot_pml4      + 0xFF8 // pml4[511] low 32 bits
	movl	$0,	boot_pml4      + 0xFFC // pml4[511] high 32 bits

	// Set up the Page Directory Pointer Table
	movl	$boot_pd_1g,	%eax
	orl	$0x03,	%eax                    // 0x03 = Present | RW
	movl	%eax,	boot_pdpt_low  + 0x000  // pdpt_low[0] low 32 bits
	movl	$0,	boot_pdpt_low  + 0x004  // pdpt_low[0] high 32 bits

	movl	$boot_pd_2g, %eax
	orl	$0x03,	%eax                    // 0x03 = Present | RW
	movl	%eax,	boot_pdpt_low  + 0x008  // pdpt_low[1] low 32 bits
	movl	$0,	boot_pdpt_low  + 0x012  // pdpt_low[1] high 32 bits

	movl	$boot_pd_1g,	%eax
	orl	$0x03,	%eax                    // 0x03 = Present | RW
	movl	%eax,	boot_pdpt_high + 0xFF0  // pdpt_high[510] low 32 bits
	movl	$0,	boot_pdpt_high + 0xFF4  // pdpt_high[510] high 32 bits

	movl	$boot_pd_2g, %eax
	orl	$0x03,   %eax                   // 0x03 = Present | RW
	movl	%eax,	boot_pdpt_high + 0xFF8  // pdpt_high[511] low 32 bits
	movl	$0,	boot_pdpt_high + 0xFFC  // pdpt_high[511] high 32 bits

	// Set up the Directories
	movl	$512,	%ecx
	xorl	%eax,	%eax // physical address
	movl	$0x83,	%ebx // 0x83 = Page Size | Present | RW
	xorl	%edx,	%edx
	movl	$boot_pd_1g,	%edi
2:
	movl	%eax,	%esi
	orl	%ebx,	%esi
	movl	%esi,	(%edi,%edx,8)
	movl	$0,	4(%edi,%edx,8)
	add	$0x200000,	%eax // Next 2M page
	incl	%edx
	decl	%ecx
	jnz	2b

	movl	$512,	%ecx
	xorl	%edx,	%edx
	movl	$boot_pd_2g,	%edi
3:
	movl	%eax,	%esi
	orl	%ebx,	%esi
	movl	%esi,	(%edi,%edx,8)
	movl	$0,	4(%edi,%edx,8)
	add	$0x200000,	%eax // Next 2M page
	incl	%edx
	decl	%ecx
	jnz	3b

	// Enable Physical Address Extension
	movl	%cr4,	%eax
	orl	$0x20,	%eax // 1 << 5 = 0x20 = PAE bit
	movl	%eax,	%cr4

	// Set the pml4 address
	movl	$boot_pml4,	%eax
	movl	%eax,	%cr3

	// Set Long Mode Enable
	movl	$0xC0000080,	%ecx
	rdmsr
	orl	$0x100,	%eax // 1 << 8 = 0x100 = LME bit
	wrmsr

	// Now enable paging
	movl	%cr0,	%eax
	orl	$0x80000000,	%eax // 1 << 31 = 0x80000000 = PG bit
	movl	%eax,	%cr0

	movw	$LONG_MODE_TRANSITION_GDT_DATA64_INDEX,	%ax
	movw	%ax,	%ss
	movw	%ax,	%ds
	movw	%ax,	%es
	movw	%ax,	%fs
	movw	%ax,	%gs
	ljmp	$LONG_MODE_TRANSITION_GDT_CODE64_INDEX,	$4f
.code64
4:
	movabs	$multiboot_long_mode_enable,	%rax
	jmp	*%rax
.cfi_endproc
.size multiboot_entry,.-multiboot_entry

.section .text
.code64
.align 8
.global multiboot_long_mode_enable
.type multiboot_long_mode_enable,@function
multiboot_long_mode_enable:
.cfi_startproc
.cfi_undefined rip
	movq	$kernel_stack_top,	%rsp
	movq	%rsp,	%rbp
.cfi_def_cfa_register %rbp
	movl	(mb_magic),	%eax
	movl	%eax,	%edi
	movl	(mb_info_addr),	%eax
	movl	%eax,	%esi
	call	kernel_main
.cfi_endproc
	cli
1:
	hlt
	jmp	1b
.size multiboot_long_mode_enable,.-multiboot_long_mode_enable

.section .boot.bss,"aw",@nobits
.align 16
.global stack_bottom
stack_bottom:
	.skip 0x1000
.global stack_top
stack_top:

.global mb_magic
mb_magic:
	.long 0
.global mb_info_addr
mb_info_addr:
	.long 0

.align 0x1000
.global boot_pml4
.type boot_pml4,@object
.size boot_pml4, 0x1000
boot_pml4:
	.skip 0x1000

.global boot_pdpt_low
.type boot_pdpt_low,@object
.size boot_pdpt_low, 0x1000
boot_pdpt_low:
	.skip 0x1000

.global boot_pdpt_high
.type boot_pdpt_high,@object
.size boot_pdpt_high, 0x1000
boot_pdpt_high:
	.skip 0x1000

.global boot_pd_1g
.type boot_pd_1g,@object
.size boot_pd_1g, 0x1000
boot_pd_1g:
	.skip 0x1000

.global boot_pd_2g
.type boot_pd_2g,@object
.size boot_pd_2g, 0x1000
boot_pd_2g:
	.skip 0x1000
boot_pt_end:

.global alloc_pd
.type alloc_pd,@object
.size alloc_pd, 0x1000
alloc_pd:
	.skip 0x1000

.global alloc_pt
.type alloc_pt,@object
.size alloc_pt, 0x1000
alloc_pt:
	.skip 0x1000

.global BOOT_PT_LENGTH
.set BOOT_PT_LENGTH, boot_pt_end-boot_pml4

.section .bss
.align 16
.global kernel_stack_bottom
kernel_stack_bottom:
	.skip 0x4000
.global kernel_stack_top
kernel_stack_top:


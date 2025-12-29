// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#define __ASSEMBLY__
#include <boot/multiboot2.h>

.extern multiboot_entry

.set FLAGS, 0
.set MAGIC, MULTIBOOT2_HEADER_MAGIC
.set SIZE, multiboot_header_end - multiboot_header_start
.set CHECKSUM, -(MAGIC + FLAGS + SIZE)

.section .multiboot
.align 8
.global multiboot_header_start
multiboot_header_start:
	.long MULTIBOOT2_HEADER_MAGIC
	.long FLAGS
	.long SIZE
	.long CHECKSUM
entry_address_tag_start:
	.short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long entry_address_tag_end - entry_address_tag_start
	.long multiboot_entry           // entry_addr
entry_address_tag_end:
.align 8 // The header needs to be padded to align to an 8-byte boundry
framebuffer_tag_start:
	.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long framebuffer_tag_end - framebuffer_tag_start
	.long 1024                      // widtth
	.long 768                       // height
	.long 32                        // depth
framebuffer_tag_end:
.align 8 // The header needs to be padded to align to an 8-byte boundry
	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long 8
.global multiboot_header_end
multiboot_header_end:


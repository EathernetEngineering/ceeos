// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.set FLAGS, 0
.set MAGIC, 0xE85250D6
.set SIZE, multiboot_header_end - multiboot_header_start
.set CHECKSUM, -(MAGIC + FLAGS + SIZE)

.set MULTIBOOT_TAG_TYPE_END, 0
.set MULTIBOOT_TAG_TYPE_CMDLINE, 1
.set MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME, 2
.set MULTIBOOT_TAG_TYPE_MODULE, 3
.set MULTIBOOT_TAG_TYPE_BASIC_MEMINFO, 4
.set MULTIBOOT_TAG_TYPE_BOOTDEV, 5
.set MULTIBOOT_TAG_TYPE_MMAP, 6
.set MULTIBOOT_TAG_TYPE_VBE, 7
.set MULTIBOOT_TAG_TYPE_FRAMEBUFFER, 8
.set MULTIBOOT_TAG_TYPE_ELF_SECTIONS, 9
.set MULTIBOOT_TAG_TYPE_APM, 10
.set MULTIBOOT_TAG_TYPE_EFI32, 11
.set MULTIBOOT_TAG_TYPE_EFI64, 12
.set MULTIBOOT_TAG_TYPE_SMBIOS, 13
.set MULTIBOOT_TAG_TYPE_ACPI_OLD, 14
.set MULTIBOOT_TAG_TYPE_ACPI_NEW, 15
.set MULTIBOOT_TAG_TYPE_NETWORK, 16
.set MULTIBOOT_TAG_TYPE_EFI_MMAP, 17
.set MULTIBOOT_TAG_TYPE_EFI_BS, 18
.set MULTIBOOT_TAG_TYPE_EFI32_IH, 19
.set MULTIBOOT_TAG_TYPE_EFI64_IH, 20
.set MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR, 21

.set MULTIBOOT_HEADER_TAG_END, 0
.set MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST, 1
.set MULTIBOOT_HEADER_TAG_ADDRESS, 2
.set MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS, 3
.set MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS, 4
.set MULTIBOOT_HEADER_TAG_FRAMEBUFFER, 5
.set MULTIBOOT_HEADER_TAG_MODULE_ALIGN, 6
.set MULTIBOOT_HEADER_TAG_EFI_BS, 7
.set MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32, 8
.set MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64, 9
.set MULTIBOOT_HEADER_TAG_RELOCATABLE, 10

.set MULTIBOOT_HEADER_TAG_OPTIONAL, 1

.section .multiboot
.align 8
.global multiboot_header_start
multiboot_header_start:
	.long MAGIC
	.long FLAGS
	.long SIZE
	.long CHECKSUM
address_tag_start:
	.short MULTIBOOT_HEADER_TAG_ADDRESS
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long address_tag_end - address_tag_start
	.long multiboot_header_start // header_addr
	.long _text_start            // load_addr
	.long _edata                 // load_end__addr
	.long _end                   // bss_end_addr
address_tag_end:
.align 8 // The header needs to be padded to align to an 8-byte boundry
entry_address_tag_start:
	.short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long entry_address_tag_end - entry_address_tag_start
	.long multiboot_entry        // entry_addr
entry_address_tag_end:
.align 8 // The header needs to be padded to align to an 8-byte boundry
framebuffer_tag_start:
	.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long framebuffer_tag_end - framebuffer_tag_start
	.long 1024                   // widtth
	.long 768                    // height
	.long 32                     // depth
framebuffer_tag_end:
.align 8 // The header needs to be padded to align to an 8-byte boundry
	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long 8
.global multiboot_header_end
multiboot_header_end:


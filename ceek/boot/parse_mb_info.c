// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include "acpi.h"
#include <boot/parse_mb_info.h>
#include <boot/multiboot2.h>

#include <io/kprint.h>
#include <mmap.h>

int parse_boot_info(uintptr_t addr)
{
	if (addr & 7) {
		return -1;
	}
	size_t length = *(size_t *)addr; (void)length;
	struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);

	while (tag->type != MULTIBOOT_TAG_TYPE_END) {
		switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				kprint("Command line: ");
				kprint(((struct multiboot_tag_string *)tag)->string);
				kprint("\r\n");
				//TODO: Handle command line
				break;

			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				kprint("Booted by: ");
				kprint(((struct multiboot_tag_string *)tag)->string);
				kprint("\r\n");
				//TODO: Save this?
				break;

			case MULTIBOOT_TAG_TYPE_MODULE:
				kprint(((struct multiboot_tag_module *)tag)->cmdline);
				kprint("\r\n");
				// TODO: Handle modules
				break;

			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				mmap_set_mb_basic_minfo((struct multiboot_tag_basic_meminfo *)tag);
				break;

			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				// TODO: Handle boot drive
				break;

			case MULTIBOOT_TAG_TYPE_MMAP:
				mmap_set_mb_mmap((struct multiboot_tag_mmap *)tag);
				break;

			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
				// TODO: Start using the framebuffer!!
				break;

			case MULTIBOOT_TAG_TYPE_ACPI_OLD: {
				uintptr_t addr;
				// TODO: Do the checksum and extract the read address from this copy
				addr = (uintptr_t)((struct multiboot_tag_old_acpi *)tag)->rsdp;
				acpi_set_rsdpv1(addr);
			} break;

			case MULTIBOOT_TAG_TYPE_ACPI_NEW: {
				uintptr_t addr;
				// TODO: Do the checksum and extract the read address from this copy
				addr = (uintptr_t)((struct multiboot_tag_new_acpi *)tag)->rsdp;
				acpi_set_rsdpv2(addr);
			} break;

			case MULTIBOOT_TAG_TYPE_NETWORK:
				// TODO: Handle network
				break;

			case MULTIBOOT_TAG_TYPE_EFI_MMAP:
				mmap_set_efi_mmap((struct multiboot_tag_efi_mmap *)tag);
				break;

			default:
				break;
		}

		tag = (struct multiboot_tag *)((uintptr_t)tag + (((tag->size + 7) & ~7)));
	}

	return 0;
}


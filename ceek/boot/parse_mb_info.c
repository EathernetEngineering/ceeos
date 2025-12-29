// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <boot/parse_mb_info.h>

#include <acpi.h>
#include <elf.h>
#include <mmap.h>
#include <boot/multiboot2.h>
#include <io/kprint.h>

int parse_boot_info(uintptr_t addr, struct boot_info *info)
{
	// Multiboot2 spec requires 8-byte alignment
	if (addr & 0x07) {
		return -1;
	}
	size_t length = *(size_t *)addr; (void)length;
	struct multiboot_tag *tag = (struct multiboot_tag *)(addr + 8);

	while (tag->type != MULTIBOOT_TAG_TYPE_END) {
		switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				info->cmdline = ((struct multiboot_tag_string *)tag)->string;
				break;

			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				info->boot_loader_name = ((struct multiboot_tag_string *)tag)->string;
				break;

			case MULTIBOOT_TAG_TYPE_MODULE:
				kprint(((struct multiboot_tag_module *)tag)->cmdline);
				kprint("\r\n");
				// TODO: Handle modules
				break;

			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				info->basic_meminfo = (void *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				// TODO: Handle boot drive
				break;

			case MULTIBOOT_TAG_TYPE_MMAP:
				info->mmap = (void *)tag;
				break;

			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
				// TODO: Start using the framebuffer!!
				struct multiboot_tag_framebuffer *fbtag = (struct multiboot_tag_framebuffer *)tag;
				info->framebuffer.phys_addr = fbtag->common.framebuffer_addr;
				info->framebuffer.width = fbtag->common.framebuffer_width;;
				info->framebuffer.height = fbtag->common.framebuffer_height;
				info->framebuffer.pitch = fbtag->common.framebuffer_pitch;
				info->framebuffer.bpp = fbtag->common.framebuffer_bpp;
				info->framebuffer.type = fbtag->common.framebuffer_type;
				if (fbtag->common.framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {
					struct multiboot_frambuffer_rgb *colorinfo =
						(struct multiboot_frambuffer_rgb *)fbtag->framebuffer_color_info;

					info->framebuffer.rgb.red_mask_size = colorinfo->framebuffer_red_mask_size;
					info->framebuffer.rgb.red_mask_shift = colorinfo->framebuffer_red_field_position;
					info->framebuffer.rgb.green_mask_size = colorinfo->framebuffer_green_mask_size;
					info->framebuffer.rgb.green_mask_shift = colorinfo->framebuffer_green_field_position;
					info->framebuffer.rgb.blue_mask_size = colorinfo->framebuffer_blue_mask_size;
					info->framebuffer.rgb.blue_mask_shift = colorinfo->framebuffer_blue_field_position;
				} else if (fbtag->common.framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED) {
					struct multiboot_frambuffer_palette *paletteinfo =
						(struct multiboot_frambuffer_palette *)fbtag->framebuffer_color_info;

					info->framebuffer.indexed.palette_num_colors = paletteinfo->framebuffer_palette_num_colors;
					info->framebuffer.indexed.palette = (void *)paletteinfo->framebuffer_palette;
				}
			} break;

			case MULTIBOOT_TAG_TYPE_ACPI_OLD: {
				info->rsdpv1 = (uintptr_t)((struct multiboot_tag_old_acpi *)tag)->rsdp;
			} break;

			case MULTIBOOT_TAG_TYPE_ACPI_NEW: {
				info->rsdpv2 = (uintptr_t)((struct multiboot_tag_old_acpi *)tag)->rsdp;
			} break;

			case MULTIBOOT_TAG_TYPE_NETWORK:
				// TODO: Handle network
				break;

			case MULTIBOOT_TAG_TYPE_EFI_MMAP:
				info->efi_mmap = ((struct multiboot_tag_efi_mmap *)tag);
				break;

			case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
				info->elf_sections.num = ((struct multiboot_tag_elf_sections *)tag)->num;
				info->elf_sections.entry_size = ((struct multiboot_tag_elf_sections *)tag)->entsize;
				info->elf_sections.shndx = ((struct multiboot_tag_elf_sections *)tag)->shndx;
				info->elf_sections.sections =
					(struct elf_section_header *)((struct multiboot_tag_elf_sections *)tag)->sections;
				break;

			default:
				break;
		}

		tag = (struct multiboot_tag *)((uintptr_t)tag + (((tag->size + 7) & ~7)));
	}

	return 0;
}


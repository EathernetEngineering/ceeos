// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_BOOT_EFI_H_
#define CEEK_BOOT_EFI_H_

#include <stdint.h>

#define EFI_MEMORY_UC              0x0000000000000001
#define EFI_MEMORY_WC              0x0000000000000002
#define EFI_MEMORY_WT              0x0000000000000004
#define EFI_MEMORY_WB              0x0000000000000008
#define EFI_MEMORY_UCE             0x0000000000000010
#define EFI_MEMORY_WP              0x0000000000001000
#define EFI_MEMORY_RP              0x0000000000002000
#define EFI_MEMORY_XP              0x0000000000004000
#define EFI_MEMORY_NV              0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE   0x0000000000010000
#define EFI_MEMORY_RO              0x0000000000020000
#define EFI_MEMORY_SP              0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO      0x0000000000080000
#define EFI_MEMORY_HOT_PLUGGABLE   0x0000000000100000
#define EFI_MEMORY_RUNTIME         0x8000000000000000
#define EFI_MEMORY_ISA_VALID       0x4000000000000000
#define EFI_MEMORY_ISA_MASK        0x0FFFF00000000000

enum efi_memory_type {
	EFI_RESERVED_MEMORY_TYPE        = 0,
	EFI_LOADER_CODE                 = 1,
	EFI_LOADER_DATA                 = 2,
	EFI_BOOT_SERVICES_CODE          = 3,
	EFI_BOOT_SERVICES_DATA          = 4,
	EFI_RUNTIME_SERVICES_CODE       = 5,
	EFI_RUNTIME_SERVICES_DATA       = 6,
	EFI_CONVENTIONAL_MEMORY         = 7,
	EFI_UNUSABLE_MEMORY             = 8,
	EFI_ACPI_RECLAIM_MEMORY         = 9,
	EFI_ACPI_MEMORY_NVS             = 10,
	EFI_MEMORY_MAPPED_IO            = 11,
	EFI_MEMORY_MAPPED_IO_PORT_SPACE = 12,
	EFI_PAL_CODE                    = 13,
	EFI_PERSISTENT_MEMORY           = 14,
	EFI_UNACCEPTED_MEMORY_TYPE      = 15,
	EFI_MAX_MEMORY_TYPE             = 16
};

struct efi_mmap_entry {
	uint32_t type;
	uint32_t zero;
	uint64_t paddr_start;
	uint64_t vaddr_start;
	uint64_t page_count;
	uint64_t attributes;
};

#endif


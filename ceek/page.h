// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_PAGE_H_
#define CEEK_PAGE_H_

#include <stdint.h>

#define PTE_P            (1ull <<  0) // Present
#define PTE_RW           (1ull <<  1) // Read/Write
#define PTE_US           (1ull <<  2) // User/Supervisor
#define PTE_PWT          (1ull <<  3) // Write Though
#define PTE_PCD          (1ull <<  4) // Cache Disable
#define PTE_PA           (1ull <<  5) // Accessed
#define PTE_PD           (1ull <<  6) // Dirty
#define PTE_PS           (1ull <<  7) // Page Size
#define PTE_PG           (1ull <<  8) // Global
#define PTE_NX           (1ull << 63) // No Execute

#define PTE_ADDRESS_MASK 0x000FFFFFFFFFF000ull
#define PTE_FLAGS_MASK   0xF000000000000FFFull

#define PTE_KERNEL_RW (PTE_P | PTE_RW)
#define PTE_KERNEL_RX (PTE_P)
#define PTE_USER_RW (PTE_P | PTE_RW | PTE_US)
#define PTE_USER_RX (PTE_P | PTE_US)

typedef uint64_t page_table_entry_t;
typedef struct page_table {
	page_table_entry_t entires[512];
} __attribute__((aligned(4096))) page_table_t;

#endif


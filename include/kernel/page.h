// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_PAGE_H_
#define CEEK_PAGE_H_

#include <lib/stdint.h>

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

#define PAGE_SIZE_4K 0x1000ull
#define PAGE_SIZE_2M 0x200000ull
#define PAGE_SIZE_1G 0x40000000ull

#define PML4_IDX_SHIFT 39
#define PDPT_IDX_SHIFT 30
#define PD_IDX_SHIFT 21
#define PT_IDX_SHIFT 12

#define PTE_IDX_MASK 0x1FFull // bits [8..0] for 512 entries

#define ALIGN_UP(val, align) (((val) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(val, align) ((val) & ~((align) - 1))

typedef uint64_t page_table_entry_t;
struct __attribute__((aligned(4096))) page_table {
	page_table_entry_t entries[512];
};
typedef struct page_table page_table_t;

static inline page_table_entry_t pte_create(uintptr_t vaddr, uint64_t flags)
{
	return (vaddr & PTE_ADDRESS_MASK) | (flags & PTE_FLAGS_MASK);
}

static inline uintptr_t pte_addr(page_table_entry_t e)
{
	return e & PTE_ADDRESS_MASK;
}

static inline int pte_is_present(page_table_entry_t e)
{
	return !!(e & PTE_P);
}

static inline uint16_t pml4_index(uintptr_t vaddr)
{
	return (vaddr >> PML4_IDX_SHIFT) & PTE_IDX_MASK;
}

static inline uint16_t pdpt_index(uintptr_t vaddr)
{
	return (vaddr >> PDPT_IDX_SHIFT) & PTE_IDX_MASK;
}

static inline uint16_t pd_index(uintptr_t vaddr)
{
	return (vaddr >> PD_IDX_SHIFT) & PTE_IDX_MASK;
}

static inline uint16_t pt_index(uintptr_t vaddr)
{
	return (vaddr >> PT_IDX_SHIFT) & PTE_IDX_MASK;
}

uintptr_t pml4_get(void);
void pml4_set(uintptr_t paddr);

void invlpg(uintptr_t vaddr);
void tlb_invalidate(void);

#endif


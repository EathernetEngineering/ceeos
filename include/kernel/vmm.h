// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_VMM_H_
#define CEEK_VMM_H_

#include <arch/x86/interrupts.h>

#include <lib/stdint.h>

#define PHYSMAP_BASE 0xFFFF800000000000ull
#define KERNEL_BASE  0xFFFFFFFF80000000ull

enum page_size {
	PAGE_SIZE_4K_ENUM = 0,
	PAGE_SIZE_2M_ENUM = 1,
	PAGE_SIZE_1G_ENUM = 2
};

struct address_space {
	uintptr_t pml4_phys;
};

__attribute__((warn_unused_result))
int __init_paging(void);

int vmm_map_range(struct address_space *as,
		uintptr_t vaddr, uintptr_t paddr, size_t length,
		uint64_t flags, enum page_size max_ps);
int vmm_map_page(struct address_space *as,
		uintptr_t vaddr, uintptr_t paddr,
		uint64_t flags, enum page_size ps);
int vmm_unmap_page(struct address_space *as, uintptr_t vaddr);

static inline uintptr_t vmm_virt_to_phys(void *virt)
{
	return (uintptr_t)virt - PHYSMAP_BASE;
}

static inline void *vmm_phys_to_virt(uintptr_t phys)
{
	return (void *)(PHYSMAP_BASE + phys);
}

void vmm_handle_pf(const struct isr_context *ctx);

#endif


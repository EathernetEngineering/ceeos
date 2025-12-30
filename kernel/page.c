// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <kernel/page.h>

uintptr_t pml4_get(void)
{
	uintptr_t paddr;
	__asm__ volatile ("movq %%cr3, %0" : "=r"(paddr));
	return paddr;
}

void pml4_set(uintptr_t paddr)
{
	__asm__ volatile ("movq %0, %%cr3" :: "r"(paddr) : "memory");
}

void invlpg(uintptr_t vaddr)
{
	__asm__ volatile ("invlpg (%0)" :: "r"(vaddr) : "memory");
}

void tlb_invalidate(void)
{
	__asm__ volatile ("movq %%cr3, %%rax\n"
			"movq %%rax, %%cr3"
			::: "%rax", "memory");
}


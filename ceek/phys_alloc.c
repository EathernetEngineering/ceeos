// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <phys_alloc.h>
#include <boot/page_table.h>

#include <stdlib.h>

uintptr_t phys_alloc_page(void)
{
	return 0;
}

void phys_alloc_free_page(uintptr_t page)
{
	(void)page;
}

void *vmm_map_phys(uintptr_t phys, size_t len)
{
	(void)phys, (void)len;
	return NULL;
}

void vmm_unmap_phys(uintptr_t phys)
{
	(void)phys;
}


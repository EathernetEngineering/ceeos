// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_PHYS_ALLOC_H_
#define CEEK_PHYS_ALLOC_H_

#include <page.h>
#include <mmap.h>

void __init_phys_alloc_bump(const struct pmmap *mmap);
__attribute__((warn_unused_result)) uintptr_t phys_alloc_bump(void);
__attribute__((warn_unused_result)) uintptr_t phys_alloc_bump_n(size_t n);

__attribute__((warn_unused_result)) uintptr_t phys_page_alloc(void);
void phys_page_free(uintptr_t page);

void phys_alloc_init(struct pmmap *mmap);

#endif


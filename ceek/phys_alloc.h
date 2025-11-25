// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_PHYS_ALLOC_H_
#define CEEK_PHYS_ALLOC_H_

#include <page.h>

uintptr_t phys_alloc_page(void);
void phys_alloc_free_page(uintptr_t page);

#endif


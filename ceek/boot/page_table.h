// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_BOOT_PAGE_TABLE_H_
#define CEEK_BOOT_PAGE_TABLE_H_

#include <page.h>

extern page_table_t pml4;
extern page_table_t pdpt_low;
extern page_table_t pdpt_high;
extern page_table_t pd_1g;
extern page_table_t pd_2g;

#endif


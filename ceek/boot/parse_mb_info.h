// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_BOOT_PARSE_BM_INFO_H_
#define CEEK_BOOT_PARSE_BM_INFO_H_

#include <stdint.h>
#include <boot/multiboot2.h>

int parse_boot_info(uintptr_t addr);

#endif


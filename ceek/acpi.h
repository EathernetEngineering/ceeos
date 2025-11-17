// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_ACPI_H_
#define CEEK_ACPI_H_

#include <stdint.h>

struct __attribute__((packed)) acpi_rsdp {
	char signature[8];
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;
	uint32_t rsdt_address;
};

struct __attribute__((packed)) acpi_xsdp {
	char signature[8];
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;
	uint32_t rsdt_address; // deprecated since version 2.0

	uint32_t length;
	uint64_t xsdtaddress;
	uint8_t extended_checksum;
	uint8_t reserved[3];
};

int acpi_init(void);

void acpi_set_rsdpv1(uintptr_t addr);
void acpi_set_rsdpv2(uintptr_t addr);

#endif


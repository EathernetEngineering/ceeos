// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <acpi.h>

static struct acpi_rsdp *g_rdspv1;
static struct acpi_rsdp *g_rdspv2;

int acpi_init(void)
{
	// TODO: Parse ACPI info
	return 0;
}

void acpi_set_rsdpv1(uintptr_t addr)
{
	g_rdspv1 = (struct acpi_rsdp *)addr;
}

void acpi_set_rsdpv2(uintptr_t addr)
{
	g_rdspv2 = (struct acpi_rsdp *)addr;
}


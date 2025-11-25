// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#include <acpi.h>
#include <io/kprint.h>

#include <string.h>
#include <stdlib.h>

static struct acpi_rsdp *g_mb_rsdpv1;
static struct acpi_rsdp *g_mb_rsdpv2;

static struct acpi_rsdp *g_rsdp;
static struct acpi_rsdp *g_rsdt;
static struct acpi_fadt *g_fadt;
static struct acpi_madt *g_madt;

static struct acpi_rsdp *search_for_rsdp(void);
static int verify_sdt_checksum(struct acpi_sdt_header *h);
static enum acpi_sdt_type get_sdt_signature(struct acpi_sdt_header *h);
static int parse_rsdt(void);
static int parse_xsdt(void);

int acpi_init(void)
{
	struct acpi_rsdp *rsdp = NULL;
	uint8_t checksum = 0;
	g_rsdt = NULL;
	g_fadt = NULL;
	g_madt = NULL;
	// TODO: Parse ACPI info
	if (g_mb_rsdpv2 &&
		(strncmp(g_mb_rsdpv2->signature, ACPI_RSDP_SIGNATURE, 8) == 0))
	{
		rsdp = g_mb_rsdpv2;
	} else if (g_mb_rsdpv1 &&
		(strncmp(g_mb_rsdpv1->signature, ACPI_RSDP_SIGNATURE, 8) == 0))
	{
		rsdp = g_mb_rsdpv1;
	} else {
		rsdp = search_for_rsdp();
	}
	if (!rsdp) {
		return -1;
	}

	for (int i = 0; i < (int)sizeof(struct acpi_rsdp); i++) {
		checksum += ((uint8_t*)rsdp)[i];
	}
	if (checksum != 0) {
		return -1;
	}
	g_rsdp = rsdp;
	if (rsdp->revision > 0) {
		checksum = 0;
		for (int i = 0; i < (int)sizeof(struct acpi_xsdp); i++) {
			checksum += ((uint8_t*)rsdp)[i];
		}
		if (checksum != 0) {
			return -1;
		}
		parse_xsdt();
	} else {
		parse_rsdt();
	}

	return 0;
}

void acpi_set_mb_rsdpv1(uintptr_t addr)
{
	g_mb_rsdpv1 = (struct acpi_rsdp *)addr;
}

void acpi_set_mb_rsdpv2(uintptr_t addr)
{
	g_mb_rsdpv2 = (struct acpi_rsdp *)addr;
}

static struct acpi_rsdp *search_for_rsdp(void)
{
	char *ptr = NULL;
	for (char *loc = (char *)0x000E0000ull;
		loc < (char *)0x000FFFFFull;
		loc += 16)
	{
		if (strncmp(loc, "RSD PTR ", 8) == 0) {
			ptr = loc;
			break;
		}
	}
	return (struct acpi_rsdp *)ptr;
}

static int verify_sdt_checksum(struct acpi_sdt_header *h)
{
	uint8_t sum = 0;
	for (long i = 0; i < (long)h->length; i++) {
		sum += ((uint8_t *)h)[i];
	}

	return sum == 0;
}

static enum acpi_sdt_type get_sdt_signature(struct acpi_sdt_header *h)
{
	if (!h)
		return -1;

	if (memcmp(h->signature, ACPI_RSDT_SIGNATURE, 4) == 0)
		return ACPI_RSDT;
	else if (memcmp(h->signature, ACPI_XSDT_SIGNATURE, 4) == 0)
		return ACPI_XSDT;
	else if (memcmp(h->signature, ACPI_FADT_SIGNATURE, 4) == 0)
		return ACPI_FADT;
	else if (memcmp(h->signature, ACPI_MADT_SIGNATURE, 4) == 0)
		return ACPI_MADT;
	else
		return -1;
}

static int parse_rsdt(void)
{
	int entries, ret = 0;
	struct acpi_rsdt *rsdt = (struct acpi_rsdt *)(uintptr_t)g_rsdp->rsdt;
	if (!verify_sdt_checksum((struct acpi_sdt_header *)rsdt))
		return -1;
	if (get_sdt_signature((struct acpi_sdt_header *)rsdt) != ACPI_RSDT)
		return -1;

	entries = (rsdt->h.length - sizeof(rsdt->h)) / 4;
	for (int i = 0; i < entries; i++) {
		struct acpi_sdt_header *h =
			(struct acpi_sdt_header *)(uintptr_t)rsdt->other_headers[i];

		enum acpi_sdt_type type = get_sdt_signature(h);
		switch (type) {
			case ACPI_MADT:
				if (verify_sdt_checksum(h))
					g_madt = (struct acpi_madt *)h;
				else {
					kprint("MADT Checksum failed");
					--ret;
				}
				break;

			case ACPI_FADT:
				if (verify_sdt_checksum(h))
					g_fadt = (struct acpi_fadt *)h;
				else {
					kprint("MADT Checksum failed");
					--ret;
				}
				break;

			// Don't bother handling these,
			// they are already known at this point
			case ACPI_RSDT:
			case ACPI_XSDT:
				break;

			default:
				// Do nothing for now. There are many tables to be handled
				// that I do not care for at this point
				break;
		}
	}

	return ret;
}

static int parse_xsdt(void)
{
	int entries;
	struct acpi_xsdp *xsdp = (struct acpi_xsdp *)g_rsdp;
	struct acpi_xsdt *xsdt = (struct acpi_xsdt *)xsdp->xsdt;
	if (!verify_sdt_checksum((struct acpi_sdt_header *)xsdt))
		return parse_rsdt();
	if (get_sdt_signature((struct acpi_sdt_header *)xsdt) != ACPI_XSDT)
		return parse_rsdt();

	entries = (xsdt->h.length - sizeof(xsdt->h)) / 8;
	for (int i = 0; i < entries; i++) {
	}

	return 0;
}


// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

#ifndef CEEK_ACPI_H_
#define CEEK_ACPI_H_

#include <stdint.h>

#define ACPI_RSDP_SIGNATURE "RSD PTR "
#define ACPI_RSDT_SIGNATURE "RSDT"
#define ACPI_XSDT_SIGNATURE "XSDT"
#define ACPI_FADT_SIGNATURE "FACP"
#define ACPI_MADT_SIGNATURE "APIC"

struct acpi_rsdp {
	char signature[8];
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;
	uint32_t rsdt;
} __attribute__((packed));

struct acpi_xsdp {
	char signature[8];
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;
	uint32_t rsdt_address; // deprecated since version 2.0

	uint32_t length;
	uint64_t xsdt;
	uint8_t extended_checksum;
	uint8_t reserved[3];
} __attribute__((packed));

struct acpi_sdt_header {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemid[6];
	char oem_tableid[8];
	uint32_t oem_revision;
	uint32_t creatorid;
	uint32_t creator_revision;
} __attribute__((packed));

struct acpi_rsdt {
	struct acpi_sdt_header h;
	uint32_t other_headers[];
} __attribute__((packed));

struct acpi_xsdt {
	struct acpi_sdt_header h;
	uint64_t other_headers[];
} __attribute__((packed));

enum acpi_sdt_type {
	ACPI_RSDT,
	ACPI_XSDT,
	ACPI_FADT,
	ACPI_MADT
};

enum acpi_preferred_power_mode {
	ACPI_POWER_MODE_UNSPECIFIED  = 0,
	ACPI_POWER_MODE_DESKTOP      = 1,
	ACPI_POWER_MODE_MOBILE       = 2,
	ACPI_POWER_MODE_WORKSTATION  = 3,
	ACPI_POWER_MODE_ENT_SERVER   = 4,
	ACPI_POWER_MODE_SOHO_SERVER  = 5,
	ACPI_POWER_MODE_APPLIANCE_PC = 6,
	ACPI_POWER_MODE_PERF_SERVER  = 7,
};

enum acpi_generic_address_space {
	ACPI_ADDRESS_SPACE_SYS_MEM    =  0,
	ACPI_ADDRESS_SPACE_SYS_IO     =  1,
	ACPI_ADDRESS_SPACE_PCI_CONFIG =  2,
	ACPI_ADDRESS_SPACE_EMBC       =  3,
	ACPI_ADDRESS_SPACE_SMB        =  4,
	ACPI_ADDRESS_SPACE_CMOS       =  5,
	ACPI_ADDRESS_SPACE_PCI_DBT    =  6,
	ACPI_ADDRESS_SPACE_IPMU       =  7,
	ACPI_ADDRESS_SPACE_GPIO       =  8,
	ACPI_ADDRESS_SPACE_GSB        =  9,
	ACPI_ADDRESS_SPACE_PCC        = 10
};

enum acpi_generic_address_access_size {
	ACPI_ADDRESS_ACCESS_UNDEFINED = 0,
	ACPI_ADDRESS_ACCESS_8B        = 1,
	ACPI_ADDRESS_ACCESS_16B       = 2,
	ACPI_ADDRESS_ACCESS_32B       = 3,
	ACPI_ADDRESS_ACCESS_64B       = 4,
};

struct acpi_generic_address_structure {
	uint8_t address_space;
	uint8_t bit_width;
	uint8_t bit_offset;
	uint8_t access_size;
	uint64_t address;
};

struct acpi_fadt {
	struct acpi_sdt_header h;
	uint32_t firmware_ctrl;
	uint32_t dsdt;

	uint8_t  reserved;

	uint8_t  preferred_power_management_profile;
	uint16_t SCI_interrupt;
	uint32_t SMI_command_port;
	uint8_t  Acpi_enable;
	uint8_t  Acpi_disable;
	uint8_t  S4BIOS_REQ;
	uint8_t  PSTATE_control;
	uint32_t PM1a_event_block;
	uint32_t PM1b_event_block;
	uint32_t PM1a_control_block;
	uint32_t PM1b_control_block;
	uint32_t PM2_control_block;
	uint32_t PM_timer_block;
	uint32_t GPE0_block;
	uint32_t GPE1_block;
	uint8_t  PM1_event_length;
	uint8_t  PM1_control_length;
	uint8_t  PM2_control_length;
	uint8_t  PM_timer_length;
	uint8_t  GPE0_length;
	uint8_t  GPE1_length;
	uint8_t  GPE1_base;
	uint8_t  cstate_control;
	uint16_t worstC2_latency;
	uint16_t worstC3_latency;
	uint16_t flush_size;
	uint16_t flush_stride;
	uint8_t  duty_offset;
	uint8_t  duty_width;
	uint8_t  day_alarm;
	uint8_t  month_alarm;
	uint8_t  century;

	// reserved in ACPI 1.0; used since ACPI 2.0+
	uint16_t boot_architecture_flags;

	uint8_t  reserved2;
	uint32_t flags;

	struct acpi_generic_address_structure reset_reg;

	uint8_t  reset_value;
	uint8_t  reserved3[3];

	// 64bit pointers - Available on ACPI 2.0+
	uint64_t                x_firmware_control;
	uint64_t                x_dsdt;

	struct acpi_generic_address_structure X_PM1aEventBlock;
	struct acpi_generic_address_structure X_PM1bEventBlock;
	struct acpi_generic_address_structure X_PM1aControlBlock;
	struct acpi_generic_address_structure X_PM1bControlBlock;
	struct acpi_generic_address_structure X_PM2ControlBlock;
	struct acpi_generic_address_structure X_PMTimerBlock;
	struct acpi_generic_address_structure X_GPE0Block;
	struct acpi_generic_address_structure X_GPE1Block;
} __attribute__((packed));

struct acpi_madt_entry_header {
	uint8_t type;
	uint8_t length;
};

struct madt_entry {
	struct acpi_madt_entry_header h;
} __attribute__((packed));

struct madt_local_apic {
	struct acpi_madt_entry_header h;
	uint8_t procid;
	uint8_t id;
	uint32_t flags;
} __attribute__((packed));

struct madt_io_apic {
	struct acpi_madt_entry_header h;
	uint8_t id;
	uint8_t reserved;
	uint32_t addr;
	uint32_t gsi_base;
} __attribute__((packed));

struct madt_io_apic_iso {
	struct acpi_madt_entry_header h;
	uint8_t bus_source;
	uint8_t irq_source;
	uint32_t global_system_interrupt;
	uint16_t flags;
} __attribute__((packed));

struct madt_io_apic_nmis {
	struct acpi_madt_entry_header h;
	uint8_t nmi_source;
	uint8_t reserved;
	uint16_t flags;
	uint32_t global_system_interrupt;
} __attribute__((packed));

struct madt_local_apic_nmi {
	struct acpi_madt_entry_header h;
	uint8_t apic_procid;
	uint16_t flags;
	uint8_t lint;
} __attribute__((packed));

struct madt_local_apic_addr_override {
	struct acpi_madt_entry_header h;
	uint8_t reserved[2];
	uint32_t addr;
} __attribute__((packed));

struct madt_local_x2_apic {
	struct acpi_madt_entry_header h;
	uint8_t reserved[2];
	uint32_t procid;
	uint32_t flags;
	uint32_t id;
} __attribute__((packed));

struct acpi_madt {
	struct acpi_sdt_header h;
	uint32_t local_apic_addr;
	uint32_t flags;
	struct madt_entry entires[];
} __attribute__((packed));

__attribute__((warn_unused_result))
int __init_acpi(uintptr_t rsdpv1_phys, uintptr_t rsdpv2_phys);

#endif


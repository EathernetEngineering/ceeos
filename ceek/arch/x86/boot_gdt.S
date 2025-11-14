// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Chloe Eather

.global LONG_MODE_TRANSITION_GDT_NULL_INDEX
.set LONG_MODE_TRANSITION_GDT_NULL_INDEX,   0x00
.global LONG_MODE_TRANSITION_GDT_CODE32_INDEX
.set LONG_MODE_TRANSITION_GDT_CODE32_INDEX, 0x08
.global LONG_MODE_TRANSITION_GDT_DATA32_INDEX
.set LONG_MODE_TRANSITION_GDT_DATA32_INDEX, 0x10
.global LONG_MODE_TRANSITION_GDT_CODE64_INDEX
.set LONG_MODE_TRANSITION_GDT_CODE64_INDEX, 0x18
.global LONG_MODE_TRANSITION_GDT_DATA64_INDEX
.set LONG_MODE_TRANSITION_GDT_DATA64_INDEX, 0x20

.section .rodata
.global long_mode_transition_gdt_descriptor
long_mode_transition_gdt_descriptor:
	.short long_mode_transition_gdt_end - long_mode_transition_gdt_start
	.quad long_mode_transition_gdt_start
long_mode_transition_gdt_start:
long_mode_transition_gdt_null_descriptor:
	.short 0x0000 // limit lower [0:15]
	.short 0x0000 // base lower [0:15]
	.byte  0x00 // base middle [16:23]
	.byte  0b00000000 // access
	.byte  0b00000000 // 0:3 limit [15:19] + 4:7 flags
	.byte  0x00 // base upper [24:31]
long_mode_transition_gdt_code32_descriptor:
	.short 0xFFFF // limit lower [0:15]
	.short 0x0000 // base lower [0:15]
	.byte  0x00 // base middle [16:23]
	.byte  0b10011010 // access
	.byte  0b11001111 // 0:3 limit [15:19] + 4:7 flags
	.byte  0x00 // base upper [24:31]
long_mode_transition_gdt_data32_descriptor:
	.short 0xFFFF // limit lower [0:15]
	.short 0x0000 // base lower [0:15]
	.byte  0x00 // base middle [16:23]
	.byte  0b10010010 // access
	.byte  0b11001111 // 0:3 limit [15:19] + 4:7 flags
	.byte  0x00 // base upper [24:31]
long_mode_transition_gdt_code64_descriptor:
	.short 0xFFFF // limit lower [0:15]
	.short 0x0000 // base lower [0:15]
	.byte  0x00 // base middle [16:23]
	.byte  0b10011010 // access
	.byte  0b10101111 // 0:3 limit [15:19] + 4:7 flags
	.byte  0x00 // base upper [24:31]
long_mode_transition_gdt_data64_descriptor:
	.short 0xFFFF // limit lower [0:15]
	.short 0x0000 // base lower [0:15]
	.byte  0x00 // base middle [16:23]
	.byte  0b10010010 // access
	.byte  0b11001111 // 0:3 limit [15:19] + 4:7 flags
	.byte  0x00 // base upper [24:31]
long_mode_transition_gdt_end:


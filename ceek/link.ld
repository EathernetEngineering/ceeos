ENTRY(_start)

BOOT_LMA = 1M;
KERNEL_LMA = 2M;
KERNEL_VMA = 0xffffffff80000000;

SECTIONS
{
	. = BOOT_LMA;

	.boot.text BLOCK(4K) : ALIGN(4K)
	{
		_boot_text_start = .;
		KEEP(*(.multiboot))
		. = ALIGN(8);
		*(.boot.text)
		_boot_text_end = .;
	}
	.boot.rodata BLOCK(4K) : ALIGN(4K)
	{
		_boot_rodata_start = .;
		*(.boot.rodata)
		*(.boot.cdata)
		_boot_rodata_end = .;
	}
	.boot.bss BLOCK(4K) : ALIGN(4K)
	{
		_boot_bss_start = .;
		*(.boot.bss)
		_boot_bss_end = .;
	}

	. = KERNEL_VMA + KERNEL_LMA;

	.text ALIGN(4K) : AT(KERNEL_LMA)
	{
		. = ALIGN(8);
		_text_start = .;
		*(.text)
		_text_end = .;
	}
	.rodata ALIGN(4K) : AT(KERNEL_LMA + (ADDR(.rodata) - (ADDR(.text))))
	{
		_rodata_start = .;
		*(.rodata .rodata.*)
		*(.cdata)
		_rodata_end = .;
	}
	.data ALIGN(4K) : AT(KERNEL_LMA + (ADDR(.data) - (ADDR(.text))))
	{
		_data_start = .;
		*(.data .data.*)
		_data_end = .;
	}
	.bss ALIGN(4K) :  AT(KERNEL_LMA + (ADDR(.bss) - (ADDR(.text))))
	{
		_bss_start = .;
		*(COMMON)
		*(.bss .bss.*)
		_bss_end = .;
	}
	.eh_frame ALIGN(4K) : AT(KERNEL_LMA + (ADDR(.eh_frame) - (ADDR(.text))))
	{
		_eh_frame_start = .;
		*(.eh_frame .eh_frame.*)
		_eh_frame_end = .;
	}
	.end ALIGN(4K) : AT(KERNEL_LMA + (ADDR(.end) - (ADDR(.text))))
	{
		*(.init_array .init_array.*)
		*(.fini_array .fini_array.*)
		*(.ctors .ctors.*)
		*(.dtors .dtors.*)
		*(.data.rel.ro .data.rel.ro.*)
		*(.rela .rela.*)
		*(.rel .rel.*)
		_end = .;
	}
	/DISCARD/ :
	{
		*(.gcc_except_table .gcc_except_table.*)
		*(.comment)
		*(.note .note.*)
	}

	_kernel_virt_end = .;
	_kernel_virt_base = ADDR(.text);
	_boot_phys_start = LOADADDR(.boot.text);
	_boot_phys_end = LOADADDR(.boot.bss) + SIZEOF(.boot.bss);
	_kernel_phys_start = KERNEL_LMA;
	_kernel_phys_end = LOADADDR(.end) + SIZEOF(.end);
	_kernel_virt_offset = _kernel_virt_base - _kernel_phys_start;

	_boot_text_phys_start = LOADADDR(.boot.text);
	_boot_text_phys_end = LOADADDR(.boot.text) + SIZEOF(.boot.text);
	_boot_rodata_phys_start = LOADADDR(.boot.rodata);
	_boot_rodata_phys_end = LOADADDR(.boot.rodata) + SIZEOF(.boot.rodata);
	_boot_bss_phys_start = LOADADDR(.boot.bss);
	_boot_bss_phys_end = LOADADDR(.boot.bss) + SIZEOF(.boot.bss);
	_text_phys_start = LOADADDR(.text);
	_text_phys_end = LOADADDR(.text) + SIZEOF(.text);
	_rodata_phys_start = LOADADDR(.rodata);
	_rodata_phys_end = LOADADDR(.rodata) + SIZEOF(.rodata);
	_data_phys_start = LOADADDR(.data);
	_data_phys_end = LOADADDR(.data) + SIZEOF(.data);
	_bss_phys_start = LOADADDR(.bss);
	_bss_phys_end = LOADADDR(.bss) + SIZEOF(.bss);
}


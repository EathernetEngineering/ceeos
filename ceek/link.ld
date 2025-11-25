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
		*(.rodata)
		*(.cdata)
		_rodata_end = .;
	}
	.data ALIGN(4K) : AT(KERNEL_LMA + (ADDR(.data) - (ADDR(.text))))
	{
		_data_start = .;
		*(.data)
		_data_end = .;
	}
	_edata = ADDR(.data) + SIZEOF(.data);
	.bss ALIGN(4K) :  AT(KERNEL_LMA + (ADDR(.bss) - (ADDR(.text))))
	{
		_bss_start = .;
		*(COMMON)
		*(.bss)
		_bss_end = .;
	}
	_end = ADDR(.bss) + SIZEOF(.bss);

	_boot_text_start_phys = LOADADDR(.boot.text);
	_edata_phys = LOADADDR(.data) + SIZEOF(.data);
	_end_phys = LOADADDR(.bss) + SIZEOF(.bss);

	_kernel_virt_base = ADDR(.text);
	_kernel_phys_start = KERNEL_LMA;
	_kernel_phys_end = LOADADDR(.bss) + SIZEOF(.bss);
	_kernel_virt_offset = _kernel_virt_base - _kernel_phys_start;
}


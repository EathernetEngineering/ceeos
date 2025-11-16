ENTRY(_start)

SECTIONS
{
	. = 1M;
	.text BLOCK(4K) : ALIGN(4K)
	{
		. = ALIGN(8);
		_text_start = .;
		KEEP(*(.multiboot))
		. = ALIGN(8);
		*(.text)
		_text_end = .;
	}
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		_rodata_start = .;
		*(.rodata)
		_rodata_end = .;
	}
	.data BLOCK(4K) : ALIGN(4K)
	{
		_data_start = .;
		*(.data)
		_data_end = .;
	}
	_edata = .;
	.bss BLOCK(4K) : ALIGN(4K)
	{
		_bss_start = .;
		*(COMMON)
		*(.bss)
		_bss_end = .;
	}
	_end = .;
}


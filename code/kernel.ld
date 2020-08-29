/* From http://www.lowlevel.eu/wiki/Teil_4_-_Hello_World */

ENTRY(_start)

SECTIONS
{
	. = 0x1000000;

	.text : {
		*(.text)
	}

	.data ALIGN(4096) : {
		*(.data)
	}

	.rodata ALIGN(4096) : {
		*(.rodata)
	}

	.bss ALIGN(4096) : {
		*(.bss)
	}
}
SECTIONS
{
	. = 0x0;
	.vector : {*(isr_vector)}
	. = 0x8000000;
	.text : {*(.text)}
	//.data : { *(.data) }
	//.bss : { *(.bss) }

}

SECTIONS 
	{
		. = 0x58000000;
		.test : 
		{
			*(.text)
		}
		
		. = ALIGN(4);
		.rodata :
		{
			*(.data)
		}
		
		. = ALIGN(4);	
		.data :
		{
			*(.data)
		}
		
		. = ALIGN(4);
		bss_start = .;
		.bss  : { *(.bss)  *(COMMON) }
		bss_end = .;
	}

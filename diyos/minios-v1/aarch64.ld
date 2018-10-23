ENTRY(_reset)
SECTIONS
{

	 . = 0x80008000;
	.text :
	{
	      *(.text.boot)
	      *(.text)
	}
	. = ALIGN(8);

	.rodata :
	{
		*(.rodata)
	}
	. = ALIGN(8);

	.data :
	{
		*(.data)
		/*KEEP(*(SORT(.data.*.irq_handler)))*/
		/*KEEP(*(SORT(.data.*.modules)))*/
	}
	. = ALIGN(8);

	.bss :
	{
	     __bss_start = .;
	     *(.sbss.*)
	     *(.bss*)
	     *(COMMON)
	     __bss_end = .;
	}
	. = ALIGN(8);

	secstack_base = .;
	. = . + 0x1000; /* 4kB for secure boot  EL3*/
	secstack_top = .;
	. = ALIGN(8);
	
	kernstack_base = .;
	. = . + 0x1000; /* 4kB for kernel stack EL1*/
	kernstack_top = .;
	. = ALIGN(8);

	usrstack_base = .;
	. = . + 0x1000; /* 4kB for userland stack  EL0*/
	usrstack_top = .;
	. = ALIGN(8);

	heap_start = .;
	. += 0x6400000;/* 100MB for heap*/
	heap_end = .;

}

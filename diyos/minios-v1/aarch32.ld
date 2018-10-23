ENTRY(_reset)
SECTIONS
{

	 . = 0x80008000;
	.text :
	{
	      *(.text.boot)
	      *(.text)
	}
	. = ALIGN(4);

	.rodata :
	{
		*(.rodata)
	}
	. = ALIGN(4);

	.data :
	{
		*(.data)
		/*KEEP(*(SORT(.data.*.irq_handler)))*/
		/*KEEP(*(SORT(.data.*.modules)))*/
	}
	. = ALIGN(4);

	.bss :
	{
	     __bss_start = .;
	     *(.sbss.*)
	     *(.bss*)
	     *(COMMON)
	     __bss_end = .;
	}
	. = ALIGN(4);

	kernstack_base = .;
	. = . + 0x1000; /* 4kB for kernel stack ,svc mode*/
	kernstack_top = .;
	. = ALIGN(4);

	irqstack_base = .;
	. = . + 0x1000; /* 4kB for irq stack */
	irqstack_top = .;
	. = ALIGN(4);

	fiqstack_base = .;
	. = . + 0x400; /* 1kB for fiq stack */
	fiqstack_top = .;
	. = ALIGN(4);

	abtstack_base = .;
	. = . + 0x400; /* 1kB for abt stack */
	abtstack_top = .;
	. = ALIGN(4);

	usrstack_base = .;
	. = . + 0x400; /* 1kB for userland stack */
	usrstack_top = .;
	. = ALIGN(4);

	undstack_base = .;
	. = . + 0x400; /* 1kB for undefine stack */
	undstack_top = .;
	. = ALIGN(4);

	heap_start = .;
	. += 0x6400000;/* 100MB for heap*/
	heap_end = .;

}

/********************************************************  
memory map 
  Start Address	Size	CM4 view Devices
  	0x0000_0000	64MB	EMI	
	0x0400_0000	32KB	CM4 TCM / cache	
	0x0400_8000	64KB	CM4 TCM	
	0x0401_8000	32KB	Reserved	
	0x0402_0000	896KB	Reserved
	0x0410_0000	1MB	Boot ROM CM4
	0x0420_0000	32K	SYSRAM
	0x0800_0000	128MB	SFC0
 	0x1000_0000	128MB	SW virtual memory management
	0x1800_0000	128MB	SW virtual memory management
*********************************************************/
 
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
 
/* Memory Spaces Definitions */
MEMORY
{
    ROM_BL(rx)            : ORIGIN = 0x08000000, LENGTH = 64K
    ROM_RTOS(rx)          : ORIGIN = 0x08010000, LENGTH = 3072K
    ROM_NVDM_RESERVED(rx) : ORIGIN = 0x083E0000, LENGTH = 64K
    ROM_EPO_RESERVED(rx)  : ORIGIN = 0x083F0000, LENGTH = 64K
    RAM (rwx)             : ORIGIN = 0x00000000, LENGTH = 4096K    
    VRAM(rwx)             : ORIGIN = 0x10000000, LENGTH = 4096K   
    TCM (rwx)          	  : ORIGIN = 0x04000000, LENGTH = 48K
    TCM_MD (rwx)          : ORIGIN = 0x0400C000, LENGTH = 16K    
    SYSRAM (rwx)          : ORIGIN = 0x04200000, LENGTH = 32K
    RETSRAM_MD (rwx)      : ORIGIN = 0x04300000, LENGTH = 8K
}
 
 /* Highest address of the stack */
_stack_end = ORIGIN(TCM) + LENGTH(TCM);    /* end of TCM */

/* Generate a link error if stack don't fit into TCM */
_stack_size = 0x1000; /* required amount of stack 4KB*/
 
/* stack start */
_stack_start = _stack_end - _stack_size;
 


/* Entry Point */
ENTRY(Reset_Handler)

SECTIONS
{
    . = ORIGIN(RAM);
    . = ALIGN(4);
    .text :ALIGN(4)
    {
        _text_start = .;
        Image$$TEXT$$Base = .;
	Image$$VECTOR$$TABLE$$Base = .;
        KEEP(*(.isr_vector))
        KEEP(*(.reset_handler))
        KEEP(*(.init))
        KEEP(*(.fini))
        

        Image$$TEXT$$Limit = .;
        _text_end = .;
        
    } > RAM  AT> RAM
    . = ALIGN(4);   
    _ram_noncached_rw_load = LOADADDR(.noncached_data);

    .noncached_data :ALIGN(4)
    {        
    . = ALIGN(4);
        Image$$NONCACHED_DATA$$Base = . ;
        _ram_noncached_rw_start = . ;
        
        *(.noncached_rwdata)
        
        Image$$NONCACHED_DATA$$Limit = .;
        _ram_noncached_rw_end = .; 
    } > RAM  AT> RAM
    . = ALIGN(4);
    .noncached_bss (NOLOAD) :
    {                
        Image$$NONCACHED_ZI$$Base = .;
        _ram_noncached_zi_start = .;
        
        *(.noncached_zidata)

        Image$$NONCACHED_ZI$$Limit = .;
        _ram_noncached_zi_end = .;
       
    } > RAM  AT> RAM

    . = . + ORIGIN(VRAM);
    . = ALIGN(32);
    _ram_code_load = LOADADDR(.ram_text);
    .ram_text . :ALIGN(32)
    {
        Image$$RAM_TEXT$$Base = .;
        _ram_code_start = .;
        
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.ram_code)
        *(.ram_rodata)
  
        Image$$RAM_TEXT$$Limit = .;
        _ram_code_end = .;
    } > VRAM  AT> RAM
    
    . = ALIGN(4);
    _data_load = LOADADDR(.cached_data);

    .cached_data . :ALIGN(4)
    {       
        __data_start__ = .;
        _data_start = .;
        Image$$CACHED_DATA$$RW$$Base = .;
        
        
        *(.data)
        *(.data*)
        
        . = ALIGN(4);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);
        
        . = ALIGN(4);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);
        
        _data_end = .;
       __data_end__ = .;
       
       
       Image$$CACHED_DATA$$RW$$Limit = .;
    
        
    } > VRAM  AT> RAM
    . = ALIGN(4);
    .bss. : 
    {
        _bss_start = .;
        __bss_start__ = .;
        Image$$CACHED_DATA$$ZI$$Base = .;
        
        *(.bss)
        *(.bss*)
        *(COMMON)
        
        . = ALIGN(4);
        _bss_end = .;
        __bss_end__ = .;
        Image$$CACHED_DATA$$ZI$$Limit = .;
        
    } > VRAM  AT> RAM
    
    ASSERT(_bss_end <= (ORIGIN(VRAM) + LENGTH(VRAM)),"RAM expired")
    . = ALIGN(4);
    _tcm_text_load = LOADADDR(.tcm);

    .tcm :ALIGN(4)
    {
        
        _tcm_text_start = .;
        Image$$TCM$$RO$$Base = .;
        
        *(.tcm_code)
        *(.tcm_rodata)
        Image$$TCM$$RO$$Limit = .;
        Image$$TCM$$RW$$Base = .;
        
        *(.tcm_rwdata)
        
        Image$$TCM$$RW$$Limit = .;
        _tcm_text_end = .;
    }> TCM  AT> RAM
    . = ALIGN(4);
    .tcm_bss (NOLOAD) :
    {
        _tcm_zi_start = .;
        Image$$TCM$$ZI$$Base = .;
        
        *(.tcm_zidata)
        
        _tcm_zi_end = .;
        Image$$TCM$$ZI$$Limit = .;
                
    }> TCM  AT> RAM

    . = ALIGN(4);
    _md_tcm_text_load = LOADADDR(.md_tcm);

    .md_tcm :ALIGN(4)
    {       
        _md_tcm_text_start = .;
        Image$$TCM_MD$$RO$$Base = .;
        
        *(.md_tcm_code)
        *(.md_tcm_rodata)
        Image$$TCM_MD$$RO$$Limit = .;
        Image$$TCM_MD$$RW$$Base = .;
        
        *(.md_tcm_rwdata)
        
        Image$$TCM_MD$$RW$$Limit = .;
        _md_tcm_text_end = .;
    }> TCM_MD  AT> RAM
    . = ALIGN(4);
    .md_tcm_bss (NOLOAD) :
    {
        _md_tcm_zi_start = .;
        Image$$TCM_MD$$ZI$$Base = .;
        
        *(.md_tcm_zidata)
        
        _md_tcm_zi_end = .;
        Image$$TCM_MD$$ZI$$Limit = .;
        
        
    }> TCM_MD  AT> RAM

     . = ALIGN(32);
    _md_retsram_text_load = LOADADDR(.md_retsram);
    .md_retsram :
    {
        _md_retsram_text_start = .;
        Image$$RETSRAM_MD$$RO$$Base = .;

        *(.md_retsram_code)
        *(.md_retsram_rodata)   
		*(.md_retsram_rwdata)

        _md_retsram_text_end = .;
        Image$$RETSRAM_MD$$RO$$Limit = .;
    }> RETSRAM_MD  AT> RAM

    /* use to check if the stack exceeds the total TCM size*/
    .stack :
    {
        . = ALIGN(4);
        
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _stack_size;
        . = ALIGN(4);
        
    } > TCM
    Image$$STACK$$ZI$$Base = _stack_end - _stack_size;
    Image$$STACK$$ZI$$Limit = _stack_end;
    
    /*extern linker symbol for region init */
    Image$$FLASH_CODE$$ZI$$Base = ORIGIN(ROM_BL);
    Image$$FLASH_CODE$$ZI$$Limit = ORIGIN(ROM_BL) + LENGTH(ROM_BL) + LENGTH(ROM_RTOS);
    Image$$FLASH_CODE$$ZI$$Length = LENGTH(ROM_BL) + LENGTH(ROM_RTOS);
    Load$$RAM_TEXT$$Base = LOADADDR(.ram_text);
    Load$$NONCACHED_DATA$$Base = LOADADDR(.noncached_data);
    Load$$CACHED_DATA$$Base = LOADADDR(.cached_data);
    Load$$TCM$$Base = LOADADDR(.tcm);
    Load$$TCM_MD$$Base = LOADADDR(.md_tcm);
    
    /* provide nvdm start and length for NVDM management */
    Image$$NVDM$$ZI$$Base = ORIGIN(ROM_NVDM_RESERVED);
    Image$$NVDM$$ZI$$Length = LENGTH(ROM_NVDM_RESERVED);
    
    /* provide EPO start and length for EPO management */
    Image$$EPO$$ZI$$Base = ORIGIN(ROM_EPO_RESERVED);
    Image$$EPO$$ZI$$Length = LENGTH(ROM_EPO_RESERVED);

}

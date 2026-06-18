/* STM32H735IGK6 Linker Script */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM_D1 (xrw)    : ORIGIN = 0x20000000, LENGTH = 128K
    RAM_D2 (xrw)    : ORIGIN = 0x30000000, LENGTH = 32K
    RAM_D3 (xrw)    : ORIGIN = 0x38000000, LENGTH = 16K
    ITCM (xrw)      : ORIGIN = 0x00000000, LENGTH = 64K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM_D1 AT >FLASH
    _sidata = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM_D1

    /* AEC buffers in fast RAM_D2 */
    .aec_buffers (NOLOAD) :
    {
        . = ALIGN(4);
        *(.aec_buffers)
        . = ALIGN(4);
    } >RAM_D2

    /* RTP/Jitter in RAM_D3 */
    .rtp_buffers (NOLOAD) :
    {
        . = ALIGN(4);
        *(.rtp_buffers)
        . = ALIGN(4);
    } >RAM_D3

    /* Set end of heap for _sbrk */
    end = .;
    _end = .;
    _estack = ORIGIN(RAM_D1) + LENGTH(RAM_D1);
}

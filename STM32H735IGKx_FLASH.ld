/* STM32H735IGK6 Flash Linker Script */
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
    . = ALIGN(4);
    _ebss = .;
  } >RAM_D1

  /* AEC buffers in fast RAM_D2 */
  .aec_buffers :
  {
    . = ALIGN(4);
    *(.aec_buffers)
    . = ALIGN(4);
  } >RAM_D2

  /* RTP/Jitter in RAM_D3 */
  .rtp_buffers :
  {
    . = ALIGN(4);
    *(.rtp_buffers)
    . = ALIGN(4);
  } >RAM_D3

  /* ITCM for tight loops */
  .itcm :
  {
    . = ALIGN(4);
    *(.itcm*)
    . = ALIGN(4);
  } >ITCM AT >FLASH
  _itcm_load = LOADADDR(.itcm);
  _itcm_start = ADDR(.itcm);
  _itcm_end = . ;
}

/**
  ******************************************************************************
  * @file    system_stm32h7xx.c
  * @brief   CMSIS Cortex-M7 Device Peripheral Access Layer System Source File
  ******************************************************************************
  */

#include "stm32h7xx.h"
#include <stdint.h>

#define HSE_VALUE 25000000U  /* 25 MHz */

/* Vector Table base offset field */
#define VECT_TAB_OFFSET  0x00U

uint32_t SystemCoreClock = 520000000;

const uint8_t  AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t  APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

void SystemInit(void) {
    /* Configure the Vector Table location */
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
    
    /* FPU settings */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
}

void SystemCoreClockUpdate(void) {
    SystemCoreClock = 520000000;
}

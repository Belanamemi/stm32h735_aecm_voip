/**
  ******************************************************************************
  * @file    stm32h7xx_hal_msp.c
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
  ******************************************************************************
  */

#include "stm32h7xx_hal.h"

/* ====== ASSERTION HANDLER ====== */
void assert_failed(uint8_t *file, uint32_t line) {
    /* User can add his own implementation to report the file name and line number */
    (void)file;
    (void)line;
    while (1) {
        /* Infinite loop for debugging */
    }
}

/* ====== HAL MSP INITIALIZATION ====== */
void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/* ====== I2C4 MSP ====== */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C4) {
        __HAL_RCC_I2C4_CLK_ENABLE();
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C4) {
        __HAL_RCC_I2C4_CLK_DISABLE();
    }
}

/* ====== SAI1 MSP ====== */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai) {
    if (hsai->Instance == SAI1_Block_A || hsai->Instance == SAI1_Block_B) {
        __HAL_RCC_SAI1_CLK_ENABLE();
    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef *hsai) {
    if (hsai->Instance == SAI1_Block_A || hsai->Instance == SAI1_Block_B) {
        __HAL_RCC_SAI1_CLK_DISABLE();
    }
}

/* ====== DMA MSP ====== */
void HAL_DMA_MspInit(DMA_HandleTypeDef *hdma) {
    __HAL_RCC_DMA2_CLK_ENABLE();
}

/* ====== ETH MSP ====== */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth) {
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();
}

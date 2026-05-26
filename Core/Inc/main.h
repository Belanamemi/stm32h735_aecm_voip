#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"

/* ====== SYSTEM CONFIGURATION ====== */
#define SYSTEM_CLOCK_HZ         520000000  /* 520 MHz */
#define SAMPLE_RATE_HZ          8000
#define FRAME_SIZE_SAMPLES      160        /* 20ms @ 8kHz */
#define FRAME_SIZE_BYTES        (FRAME_SIZE_SAMPLES * 2)

/* ====== GPIO DEFINITIONS ====== */
#define LED_GREEN_GPIO_Port     GPIOA
#define LED_GREEN_Pin           GPIO_PIN_6
#define LED_RED_GPIO_Port       GPIOB
#define LED_RED_Pin             GPIO_PIN_7

/* ====== FUNCTION PROTOTYPES ====== */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_SAI1_Init(void);
void MX_ETH_Init(void);
void MX_DMA_Init(void);
void MX_NVIC_Init(void);

void SystemClock_Config(void);

/* HAL Handlers */
extern I2C_HandleTypeDef hi2c1;
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;
extern ETH_HandleTypeDef heth;
extern DMA_HandleTypeDef hdma_sai1_a;
extern DMA_HandleTypeDef hdma_sai1_b;

#endif /* __MAIN_H__ */

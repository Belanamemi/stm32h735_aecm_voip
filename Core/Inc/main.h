/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdint.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* HAL Handlers */
extern I2C_HandleTypeDef hi2c4;
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;
extern ETH_HandleTypeDef heth;
extern DMA_HandleTypeDef hdma_sai1_a;
extern DMA_HandleTypeDef hdma_sai1_b;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


/* USER CODE BEGIN EFP */

void MX_NVIC_Init(void);

//LD1-GREEN-PC3
//LD2-RED-PC2
//BUTTON_BLUE(USER)-PC13


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUTTON_BLUE_Pin GPIO_PIN_13
#define BUTTON_BLUE_GPIO_Port GPIOC
#define BUTTON_BLUE_EXTI_IRQn EXTI15_10_IRQn
#define LED_RED_Pin GPIO_PIN_2
#define LED_RED_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_3
#define LED_GREEN_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* ====== SYSTEM CONFIGURATION ====== */
#define SYSTEM_CLOCK_HZ         520000000  /* 520 MHz */
#define SAMPLE_RATE_HZ          8000
#define FRAME_SIZE_SAMPLES      160        /* 20ms @ 8kHz */
//#define FRAME_SIZE_BYTES        (FRAME_SIZE_SAMPLES * 2)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

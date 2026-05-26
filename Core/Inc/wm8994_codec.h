#ifndef __WM8994_CODEC_H__
#define __WM8994_CODEC_H__

#include "stm32h7xx_hal.h"

/* ====== WM8994 I2C ADDRESS ====== */
#define WM8994_I2C_ADDR         0x34    /* 0x1A << 1 */

/* ====== WM8994 REGISTER MAP ====== */
#define WM8994_CHIP_ID_REG      0x0000
#define WM8994_POWER_REG        0x0001
#define WM8994_AIF1_CTRL1       0x0200
#define WM8994_AIF1_CTRL2       0x0201
#define WM8994_AIF1_SR          0x0210
#define WM8994_LEFT_ADC_VOL     0x0404
#define WM8994_RIGHT_ADC_VOL    0x0405
#define WM8994_LEFT_DAC_VOL     0x0610
#define WM8994_RIGHT_DAC_VOL    0x0611
#define WM8994_DAC_CTRL1        0x0600
#define WM8994_ADC_CTRL         0x0400

/**
 * @brief Initialize WM8994 codec for stereo (Headphone + Microphone)
 */
int wm8994_init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Set microphone (ADC) input gain
 */
int wm8994_set_input_gain(I2C_HandleTypeDef *hi2c, int8_t gain);

/**
 * @brief Set headphone (DAC) output gain
 */
int wm8994_set_output_gain(I2C_HandleTypeDef *hi2c, int8_t gain);

/**
 * @brief Start codec audio I/O
 */
int wm8994_start(I2C_HandleTypeDef *hi2c);

/**
 * @brief Stop codec audio I/O
 */
int wm8994_stop(I2C_HandleTypeDef *hi2c);

#endif /* __WM8994_CODEC_H__ */

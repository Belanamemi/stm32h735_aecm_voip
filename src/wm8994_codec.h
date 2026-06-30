#ifndef WM8994_CODEC_H
#define WM8994_CODEC_H

#include "stm32h7xx_hal.h"

int wm8994_init(I2C_HandleTypeDef *hi2c);
void wm8994_set_input_gain(I2C_HandleTypeDef *hi2c, int8_t gain_db);
void wm8994_set_output_gain(I2C_HandleTypeDef *hi2c, int8_t gain_db);

#endif

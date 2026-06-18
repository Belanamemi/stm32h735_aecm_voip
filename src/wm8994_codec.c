#include "wm8994_codec.h"
#include "stm32h7xx_hal.h"

int wm8994_init(I2C_HandleTypeDef *hi2c) {
    /* Placeholder WM8994 initialization */
    return 0;
}

void wm8994_set_input_gain(I2C_HandleTypeDef *hi2c, int8_t gain_db) {
    (void)hi2c;
    (void)gain_db;
}

void wm8994_set_output_gain(I2C_HandleTypeDef *hi2c, int8_t gain_db) {
    (void)hi2c;
    (void)gain_db;
}

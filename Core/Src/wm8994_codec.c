#include "wm8994_codec.h"
#include "itm_telemetry.h"
#include <string.h>

/* WM8994 I2C Write */
static int wm8994_write_reg(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t val) {
    uint8_t data[3];
    data[0] = (reg >> 8) & 0xFF;
    data[1] = reg & 0xFF;
    data[2] = val & 0xFF;
    
    return HAL_I2C_Master_Transmit(hi2c, WM8994_I2C_ADDR, data, 3, 100);
}

/* WM8994 I2C Read */
static int wm8994_read_reg(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t *val) {
    uint8_t addr[2] = {(reg >> 8) & 0xFF, reg & 0xFF};
    uint8_t data[2];
    
    if (HAL_I2C_Master_Transmit(hi2c, WM8994_I2C_ADDR, addr, 2, 100) != HAL_OK) return -1;
    if (HAL_I2C_Master_Receive(hi2c, WM8994_I2C_ADDR, data, 2, 100) != HAL_OK) return -1;
    
    *val = ((uint16_t)data[0] << 8) | data[1];
    return 0;
}

/**
 * Initialize WM8994 codec for stereo (Headphone + Microphone)
 */
int wm8994_init(I2C_HandleTypeDef *hi2c) {
    uint16_t chip_id = 0;
    
    /* Read Chip ID */
    if (wm8994_read_reg(hi2c, WM8994_CHIP_ID_REG, &chip_id) != 0) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ WM8994 I2C communication failed\n");
        return -1;
    }
    
    if (chip_id != 0x8994) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ WM8994 chip ID mismatch (got 0x%04X)\n", chip_id);
        return -1;
    }
    
    itm_printf(ITM_CHANNEL_DEBUG, "✅ WM8994 detected (ID: 0x%04X)\n", chip_id);
    
    /* Power configuration */
    wm8994_write_reg(hi2c, 0x0001, 0x0003);  /* Power up */
    
    /* AIF1 configuration for I2S stereo */
    wm8994_write_reg(hi2c, WM8994_AIF1_CTRL1, 0x0000);  /* Slave mode, I2S */
    wm8994_write_reg(hi2c, 0x0210, 0x0083);  /* 8kHz sample rate */
    
    /* ADC configuration (Microphone) */
    wm8994_write_reg(hi2c, WM8994_ADC_CTRL, 0x0010);    /* Enable ADCs */
    wm8994_write_reg(hi2c, WM8994_LEFT_ADC_VOL, 0x00DB); /* ADC gain */
    wm8994_write_reg(hi2c, WM8994_RIGHT_ADC_VOL, 0x00DB);
    
    /* DAC configuration (Headphone) */
    wm8994_write_reg(hi2c, WM8994_DAC_CTRL1, 0x0003);     /* Enable DACs */
    wm8994_write_reg(hi2c, WM8994_LEFT_DAC_VOL, 0x00C0);  /* DAC gain */
    wm8994_write_reg(hi2c, WM8994_RIGHT_DAC_VOL, 0x00C0);
    
    /* Input path (Microphone -> ADC) */
    wm8994_write_reg(hi2c, 0x0028, 0x0030);  /* IN1L/R to ADC */
    
    /* Output path (DAC -> Headphone) */
    wm8994_write_reg(hi2c, 0x0038, 0x0003);  /* DAC to OUT1L/R */
    wm8994_write_reg(hi2c, 0x0039, 0x0003);
    
    itm_printf(ITM_CHANNEL_DEBUG, "✅ WM8994 initialized (stereo I2S @ 8kHz)\n");
    return 0;
}

/**
 * Set microphone input gain
 */
int wm8994_set_input_gain(I2C_HandleTypeDef *hi2c, int8_t gain) {
    /* Gain range: -12 to +40 dB (0x00 to 0x3F in register) */
    uint16_t gain_val = (gain + 12) & 0x3F;
    wm8994_write_reg(hi2c, WM8994_LEFT_ADC_VOL, gain_val);
    wm8994_write_reg(hi2c, WM8994_RIGHT_ADC_VOL, gain_val);
    itm_printf(ITM_CHANNEL_DEBUG, "🎤 Microphone gain: %d dB\n", gain);
    return 0;
}

/**
 * Set headphone output gain
 */
int wm8994_set_output_gain(I2C_HandleTypeDef *hi2c, int8_t gain) {
    /* Gain range: -100 to +6 dB */
    uint16_t gain_val = (gain + 100) & 0xFF;
    wm8994_write_reg(hi2c, WM8994_LEFT_DAC_VOL, gain_val);
    wm8994_write_reg(hi2c, WM8994_RIGHT_DAC_VOL, gain_val);
    itm_printf(ITM_CHANNEL_DEBUG, "🎧 Headphone gain: %d dB\n", gain);
    return 0;
}

/**
 * Start codec
 */
int wm8994_start(I2C_HandleTypeDef *hi2c) {
    wm8994_write_reg(hi2c, 0x0001, 0x0003);  /* Power up */
    return 0;
}

/**
 * Stop codec
 */
int wm8994_stop(I2C_HandleTypeDef *hi2c) {
    wm8994_write_reg(hi2c, 0x0001, 0x0000);  /* Power down */
    return 0;
}

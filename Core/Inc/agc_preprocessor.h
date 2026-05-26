#ifndef __AGC_PREPROCESSOR_H__
#define __AGC_PREPROCESSOR_H__

#include <stdint.h>
#include "aecm_config.h"

/* ====== AGC (Automatic Gain Control) ====== */
typedef struct {
    float target_level_dbm0;    /* -20 dBm0 */
    float current_gain_db;
    float max_gain_db;
    float min_gain_db;
    float rms_level;
    float smoothing_factor;     /* 0.1 for gradual adaptation */
    uint32_t update_count;
} AgcProcessor_t;

/**
 * @brief Initialize AGC
 */
void agc_init(AgcProcessor_t *agc);

/**
 * @brief Process AGC on input frame
 */
void agc_process(AgcProcessor_t *agc, int16_t *frame, uint32_t len);

/**
 * @brief Get current AGC gain
 */
float agc_get_gain_db(const AgcProcessor_t *agc);

#endif /* __AGC_PREPROCESSOR_H__ */

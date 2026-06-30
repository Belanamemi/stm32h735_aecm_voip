#include "agc_preprocessor.h"
#include "itm_telemetry.h"
#include <math.h>
#include <string.h>

void agc_init(AgcProcessor_t *agc) {
    agc->target_level_dbm0 = AGC_TARGET_LEVEL_DBM0;
    agc->current_gain_db = 0.0f;
    agc->max_gain_db = AGC_MAX_GAIN_DB;
    agc->min_gain_db = AGC_MIN_GAIN_DB;
    agc->rms_level = 0.0f;
    agc->smoothing_factor = 0.1f;
    agc->update_count = 0;
}

void agc_process(AgcProcessor_t *agc, int16_t *frame, uint32_t len) {
    /* Calculate RMS level */
    float rms_squared = 0.0f;
    for (uint32_t i = 0; i < len; i++) {
        float s = (float)frame[i] / 32768.0f;
        rms_squared += s * s;
    }
    rms_squared /= (float)len;
    float rms_linear = sqrtf(rms_squared);
    
    /* Convert to dBm0 */
    float current_level_dbm0 = 20.0f * log10f(rms_linear + 1e-10f);
    
    /* Exponential smoothing */
    agc->rms_level = agc->smoothing_factor * current_level_dbm0 + 
                     (1.0f - agc->smoothing_factor) * agc->rms_level;
    
    /* Adjust gain to reach target */
    float gain_error = agc->target_level_dbm0 - agc->rms_level;
    agc->current_gain_db += gain_error * 0.01f;
    
    /* Clamp gain */
    if (agc->current_gain_db > agc->max_gain_db) 
        agc->current_gain_db = agc->max_gain_db;
    if (agc->current_gain_db < agc->min_gain_db) 
        agc->current_gain_db = agc->min_gain_db;
    
    /* Apply gain */
    float gain_linear = powf(10.0f, agc->current_gain_db / 20.0f);
    for (uint32_t i = 0; i < len; i++) {
        int32_t sample = (int32_t)((float)frame[i] * gain_linear);
        frame[i] = (sample > 32767) ? 32767 : (sample < -32768) ? -32768 : (int16_t)sample;
    }
    
    if (agc->update_count++ % 50 == 0) {
        itm_printf(ITM_CHANNEL_AGC, "AGC: gain=%.1f dB, level=%.1f dBm0\n", 
                   agc->current_gain_db, agc->rms_level);
    }
}

float agc_get_gain_db(const AgcProcessor_t *agc) {
    return agc->current_gain_db;
}

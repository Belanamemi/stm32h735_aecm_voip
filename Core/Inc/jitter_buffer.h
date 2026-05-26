#ifndef __JITTER_BUFFER_H__
#define __JITTER_BUFFER_H__

#include <stdint.h>
#include <stdbool.h>
#include "aecm_config.h"

/* ====== JITTER BUFFER STATE ====== */
typedef struct {
    int16_t buffer[JITTER_BUFFER_SIZE];
    uint32_t write_idx;
    uint32_t read_idx;
    uint32_t buffer_level_samples;
    uint32_t max_delay_samples;
    uint32_t min_delay_samples;
    uint32_t underrun_count;
    uint32_t overrun_count;
} JitterBuffer_t;

/**
 * @brief Initialize jitter buffer
 */
void jitter_buffer_init(JitterBuffer_t *jb);

/**
 * @brief Push incoming RTP frame
 */
int jitter_buffer_push(JitterBuffer_t *jb, const int16_t *frame, uint32_t len);

/**
 * @brief Pop frame for playback
 */
int jitter_buffer_pop(JitterBuffer_t *jb, int16_t *frame, uint32_t len);

/**
 * @brief Get current buffer level (samples)
 */
uint32_t jitter_buffer_level(const JitterBuffer_t *jb);

/**
 * @brief Auto-tune buffer delay (adaptive)
 */
void jitter_buffer_auto_tune(JitterBuffer_t *jb);

/**
 * @brief Get jitter statistics
 */
typedef struct {
    float mean_delay_ms;
    float jitter_ms;
    uint32_t underruns;
    uint32_t overruns;
} JitterStats_t;

void jitter_buffer_get_stats(const JitterBuffer_t *jb, JitterStats_t *stats);

#endif /* __JITTER_BUFFER_H__ */

#include "jitter_buffer.h"
#include "itm_telemetry.h"
#include <string.h>

void jitter_buffer_init(JitterBuffer_t *jb) {
    memset(jb, 0, sizeof(JitterBuffer_t));
    jb->min_delay_samples = JITTER_BUFFER_SIZE;
    jb->max_delay_samples = 0;
}

int jitter_buffer_push(JitterBuffer_t *jb, const int16_t *frame, uint32_t len) {
    if (len > JITTER_BUFFER_SIZE) return -1;
    
    for (uint32_t i = 0; i < len; i++) {
        jb->buffer[jb->write_idx] = frame[i];
        jb->write_idx = (jb->write_idx + 1) % JITTER_BUFFER_SIZE;
    }
    
    jb->buffer_level_samples += len;
    
    if (jb->buffer_level_samples > JITTER_BUFFER_SIZE) {
        jb->buffer_level_samples = JITTER_BUFFER_SIZE;
        jb->overrun_count++;
    }
    
    return 0;
}

int jitter_buffer_pop(JitterBuffer_t *jb, int16_t *frame, uint32_t len) {
    if (jb->buffer_level_samples < len) {
        jb->underrun_count++;
        memset(frame, 0, len * sizeof(int16_t));
        return -1;
    }
    
    for (uint32_t i = 0; i < len; i++) {
        frame[i] = jb->buffer[jb->read_idx];
        jb->read_idx = (jb->read_idx + 1) % JITTER_BUFFER_SIZE;
    }
    
    jb->buffer_level_samples -= len;
    return 0;
}

uint32_t jitter_buffer_level(const JitterBuffer_t *jb) {
    return jb->buffer_level_samples;
}

void jitter_buffer_auto_tune(JitterBuffer_t *jb) {
    #if JITTER_AUTO_TUNE_ENABLE
    if (jb->buffer_level_samples > jb->max_delay_samples) {
        jb->max_delay_samples = jb->buffer_level_samples;
    }
    if (jb->buffer_level_samples < jb->min_delay_samples) {
        jb->min_delay_samples = jb->buffer_level_samples;
    }
    #endif
}

void jitter_buffer_get_stats(const JitterBuffer_t *jb, JitterStats_t *stats) {
    stats->mean_delay_ms = (float)jb->buffer_level_samples / (float)SAMPLE_RATE_HZ * 1000.0f;
    stats->jitter_ms = (float)(jb->max_delay_samples - jb->min_delay_samples) / (float)SAMPLE_RATE_HZ * 1000.0f;
    stats->underruns = jb->underruns;
    stats->overruns = jb->overruns;
}

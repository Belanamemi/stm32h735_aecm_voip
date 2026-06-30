#include "jitter_buffer.h"
<<<<<<< HEAD
#include "itm_telemetry.h"
=======
>>>>>>> 9b48788c68f9afe210994e7ed4f34295090f6f8b
#include <string.h>

void jitter_buffer_init(JitterBuffer_t *jb) {
    memset(jb, 0, sizeof(JitterBuffer_t));
<<<<<<< HEAD
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
    stats->underruns = jb->underrun_count;
    stats->overruns = jb->overrun_count;
=======
    jb->target_delay = 40;  /* ms */
}

int jitter_buffer_push(JitterBuffer_t *jb, const int16_t *data, uint16_t len) {
    if (jb->write_pos >= JITTER_BUFFER_SIZE) {
        jb->write_pos = 0;
    }
    memcpy(&jb->buffer[jb->write_pos], data, len * 2);
    jb->write_pos += len;
    return 0;
}

int jitter_buffer_pop(JitterBuffer_t *jb, int16_t *data, uint16_t len) {
    if (jb->read_pos >= JITTER_BUFFER_SIZE) {
        jb->read_pos = 0;
    }
    memcpy(data, &jb->buffer[jb->read_pos], len * 2);
    jb->read_pos += len;
    return 0;
}

void jitter_buffer_auto_tune(JitterBuffer_t *jb) {
    /* Placeholder for jitter adaptation */
}

void jitter_buffer_get_stats(JitterBuffer_t *jb, JitterStats_t *stats) {
    stats->jitter_ms = 0.0f;
    stats->packets_late = 0;
>>>>>>> 9b48788c68f9afe210994e7ed4f34295090f6f8b
}

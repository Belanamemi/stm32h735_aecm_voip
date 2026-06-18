#include "jitter_buffer.h"
#include <string.h>

void jitter_buffer_init(JitterBuffer_t *jb) {
    memset(jb, 0, sizeof(JitterBuffer_t));
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
}

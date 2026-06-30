#ifndef JITTER_BUFFER_H
#define JITTER_BUFFER_H

#include <stdint.h>

#define JITTER_BUFFER_SIZE 8000

typedef struct {
    int16_t buffer[JITTER_BUFFER_SIZE];
    uint16_t write_pos;
    uint16_t read_pos;
    uint16_t target_delay;
} JitterBuffer_t;

typedef struct {
    float jitter_ms;
    uint32_t packets_late;
} JitterStats_t;

void jitter_buffer_init(JitterBuffer_t *jb);
int jitter_buffer_push(JitterBuffer_t *jb, const int16_t *data, uint16_t len);
int jitter_buffer_pop(JitterBuffer_t *jb, int16_t *data, uint16_t len);
void jitter_buffer_auto_tune(JitterBuffer_t *jb);
void jitter_buffer_get_stats(JitterBuffer_t *jb, JitterStats_t *stats);

#endif

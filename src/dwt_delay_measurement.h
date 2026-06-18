#ifndef DWT_DELAY_MEASUREMENT_H
#define DWT_DELAY_MEASUREMENT_H

#include <stdint.h>

typedef struct {
    uint32_t start;
    float elapsed_ms;
} DwtTimestamp_t;

void dwt_init(void);
void dwt_start(DwtTimestamp_t *ts);
void dwt_stop(DwtTimestamp_t *ts);

#endif

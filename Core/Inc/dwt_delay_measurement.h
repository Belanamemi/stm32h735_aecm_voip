#ifndef __DWT_DELAY_MEASUREMENT_H__
#define __DWT_DELAY_MEASUREMENT_H__

#include <stdint.h>
#include "stm32h7xx.h"

/* DWT (Data Watchpoint and Trace) for cycle-accurate timing */

typedef struct {
    uint32_t start_cycle;
    uint32_t end_cycle;
    uint32_t elapsed_cycles;
    float elapsed_us;
    float elapsed_ms;
} DwtTimestamp_t;

/**
 * @brief Initialize DWT counter (CYCCNT)
 */
void dwt_init(void);

/**
 * @brief Start cycle counter
 */
static inline void dwt_start(DwtTimestamp_t *ts) {
    ts->start_cycle = DWT->CYCCNT;
}

/**
 * @brief Stop cycle counter and calculate elapsed time
 */
static inline void dwt_stop(DwtTimestamp_t *ts) {
    ts->end_cycle = DWT->CYCCNT;
    ts->elapsed_cycles = ts->end_cycle - ts->start_cycle;
    ts->elapsed_us = (float)ts->elapsed_cycles / 520.0f;  /* 520 MHz */
    ts->elapsed_ms = ts->elapsed_us / 1000.0f;
}

/**
 * @brief Get current cycle count
 */
static inline uint32_t dwt_get_cycles(void) {
    return DWT->CYCCNT;
}

float dwt_measure_rtp_delay(void);
float dwt_measure_aecm_latency(void);

#endif /* __DWT_DELAY_MEASUREMENT_H__ */

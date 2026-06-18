#ifndef AECM_PROCESSOR_H
#define AECM_PROCESSOR_H

#include <stdint.h>

#define FRAME_SIZE_SAMPLES 160
#define FRAME_SIZE_BYTES (FRAME_SIZE_SAMPLES * 2)

typedef struct {
    float avg_erle_db;
    float avg_nlp_gain_db;
    uint32_t divergence_count;
} ErleMetrics_t;

typedef struct {
    int enable;
    ErleMetrics_t metrics;
} AecmState_t;

void aecm_init(void);
void aecm_process_frame(const int16_t *near, const int16_t *far, int16_t *out);
ErleMetrics_t* aecm_get_metrics(void);
void aecm_free(void);

#endif

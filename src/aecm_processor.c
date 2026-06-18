#include "aecm_processor.h"
#include <string.h>

static AecmState_t aecm_state = {0};

void aecm_init(void) {
    memset(&aecm_state, 0, sizeof(AecmState_t));
    aecm_state.enable = 1;
}

void aecm_process_frame(const int16_t *near, const int16_t *far, int16_t *out) {
    if (!aecm_state.enable) {
        memcpy(out, near, FRAME_SIZE_BYTES);
        return;
    }
    /* Placeholder AECM processing */
    memcpy(out, near, FRAME_SIZE_BYTES);
}

ErleMetrics_t* aecm_get_metrics(void) {
    return &aecm_state.metrics;
}

void aecm_free(void) {
    memset(&aecm_state, 0, sizeof(AecmState_t));
}

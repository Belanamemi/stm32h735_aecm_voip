#ifndef __AECM_PROCESSOR_H__
#define __AECM_PROCESSOR_H__

#include <stdint.h>
#include "aecm_config.h"

/**
 * @brief Initialize AECM subsystem
 */
void aecm_init(void);

/**
 * @brief Process one 20ms frame (AECM + AGC)
 */
void aecm_process_frame(int16_t *near_mic, int16_t *far_ref, int16_t *output);

/**
 * @brief Get ERLE metrics
 */
ErleMetrics_t* aecm_get_metrics(void);

/**
 * @brief Cleanup AECM
 */
void aecm_free(void);

#endif /* __AECM_PROCESSOR_H__ */

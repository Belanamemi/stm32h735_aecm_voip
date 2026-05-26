#ifndef __ITM_TELEMETRY_H__
#define __ITM_TELEMETRY_H__

#include <stdint.h>
#include <stdio.h>
#include "aecm_config.h"

/**
 * @brief ITM-based printf (requires SWO debug connection)
 */
int itm_printf(uint32_t channel, const char *format, ...);

/**
 * @brief ITM SWO port initialization
 */
void itm_init(void);

#endif /* __ITM_TELEMETRY_H__ */

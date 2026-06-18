#ifndef ITM_TELEMETRY_H
#define ITM_TELEMETRY_H

#include <stdint.h>

#define ITM_CHANNEL_DEBUG 0

void itm_init(void);
void itm_printf(uint32_t channel, const char *format, ...);

#endif

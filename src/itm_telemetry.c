#include "itm_telemetry.h"
#include <stdio.h>
#include <stdarg.h>

void itm_init(void) {
    /* Placeholder ITM init */
}

void itm_printf(uint32_t channel, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

#include "itm_telemetry.h"
#include <stdarg.h>
#include <string.h>

/* Cortex-M7 ITM Port Addresses */
#define ITM_STIM_PORT(n)  (*(volatile uint32_t *)(0xE0000000 + 4 * (n)))
#define ITM_TCR            (*(volatile uint32_t *)0xE0000E80)
#define ITM_TPR            (*(volatile uint32_t *)0xE0000E40)

void itm_init(void) {
    /* Unlock ITM */
    *((volatile uint32_t *)0xE0000FB0) = 0xC5ACCE55;
    
    /* Enable ITM */
    ITM_TCR |= (1UL << 0);     /* ITMENA */
    ITM_TCR |= (1UL << 1);     /* SYNCENA */
    ITM_TPR = 0;               /* All ports enabled */
    
    /* Enable DWT CYCCNT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

int itm_printf(uint32_t channel, const char *format, ...) {
    static char buffer[256];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    /* Write to ITM stimulus port */
    for (int i = 0; i < len; i++) {
        while (!(ITM_STIM_PORT(channel) & 1));  /* Wait for FIFO */
        ITM_STIM_PORT(channel) = buffer[i];
    }
    
    return len;
}

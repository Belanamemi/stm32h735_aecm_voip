#include "dwt_delay_measurement.h"
#include "stm32h7xx_hal.h"

void dwt_init(void) {
    /* Enable DWT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void dwt_start(DwtTimestamp_t *ts) {
    ts->start = DWT->CYCCNT;
}

void dwt_stop(DwtTimestamp_t *ts) {
    uint32_t end = DWT->CYCCNT;
    uint32_t cycles = end - ts->start;
    ts->elapsed_ms = (float)cycles / (520000.0f);  /* 520 MHz */
}

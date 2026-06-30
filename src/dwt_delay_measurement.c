#include "dwt_delay_measurement.h"
#include "itm_telemetry.h"

void dwt_init(void) {
    /* Unlock DWT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    
    /* Enable CYCCNT counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    
    /* Reset CYCCNT */
    DWT->CYCCNT = 0;
    
    itm_printf(ITM_CHANNEL_DEBUG, "✅ DWT initialized @ 520 MHz\n");
}

float dwt_measure_rtp_delay(void) {
    static uint32_t last_rtp_timestamp = 0;
    uint32_t current_timestamp = DWT->CYCCNT;
    
    if (last_rtp_timestamp == 0) {
        last_rtp_timestamp = current_timestamp;
        return 0.0f;
    }
    
    uint32_t delta_cycles = current_timestamp - last_rtp_timestamp;
    float delay_ms = (float)delta_cycles / (520.0f * 1000.0f);
    
    last_rtp_timestamp = current_timestamp;
    return delay_ms;
}

float dwt_measure_aecm_latency(void) {
    static DwtTimestamp_t ts = {0};
    static uint32_t call_count = 0;
    
    if (call_count++ % 2 == 0) {
        dwt_start(&ts);
        return 0.0f;
    }
    
    dwt_stop(&ts);
    return ts.elapsed_ms;
}

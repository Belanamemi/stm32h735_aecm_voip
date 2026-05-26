#include "aecm_processor.h"
#include "dwt_delay_measurement.h"
#include "agc_preprocessor.h"
#include "itm_telemetry.h"
#include <string.h>
#include <math.h>

/* WebRTC AECM stub functions (to be linked with actual library) */
extern void *WebRtcAecm_Create(void);
extern int WebRtcAecm_Init(void *aecmInst, int32_t sampleRate);
extern int WebRtcAecm_BufferFarend(void *aecmInst, const int16_t *farend, size_t nrSamples);
extern int WebRtcAecm_Process(void *aecmInst, const int16_t *nearendNoisy, const int16_t *nearendClean,
                             int16_t *out, int16_t msInSndCardBuf);
extern int WebRtcAecm_set_config(void *aecmInst, int echoMode);
extern void WebRtcAecm_Free(void *aecmInst);

/* Global instances */
static void *aecm_inst = NULL;
static AgcProcessor_t agc;
static ErleMetrics_t erle_metrics;
static int16_t far_buffer[FAR_END_BUFFER_SIZE];
static uint32_t far_write_idx = 0;

void aecm_init(void) {
    aecm_inst = WebRtcAecm_Create();
    if (!aecm_inst) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ AECM create failed\n");
        return;
    }
    
    if (WebRtcAecm_Init(aecm_inst, SAMPLE_RATE_HZ) != 0) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ AECM init failed\n");
        return;
    }
    
    if (WebRtcAecm_set_config(aecm_inst, AECM_MODE_CURRENT) != 0) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ AECM config failed\n");
        return;
    }
    
    agc_init(&agc);
    memset(&erle_metrics, 0, sizeof(ErleMetrics_t));
    erle_metrics.min_erle_db = 1e10;
    erle_metrics.max_erle_db = -1e10;
    
    itm_printf(ITM_CHANNEL_DEBUG, "✅ AECM initialized (8 kHz, mode=%d)\n", AECM_MODE_CURRENT);
}

void aecm_process_frame(int16_t *near_mic, int16_t *far_ref, int16_t *output) {
    DwtTimestamp_t dwt_ts;
    dwt_start(&dwt_ts);
    
    /* 1. Pre-AECM AGC on microphone input */
    int16_t agc_frame[FRAME_SIZE_SAMPLES];
    memcpy(agc_frame, near_mic, FRAME_SIZE_BYTES);
    agc_process(&agc, agc_frame, FRAME_SIZE_SAMPLES);
    
    /* 2. Store far-end reference */
    memcpy(&far_buffer[far_write_idx], far_ref, FRAME_SIZE_BYTES);
    far_write_idx = (far_write_idx + FRAME_SIZE_SAMPLES) % FAR_END_BUFFER_SIZE;
    
    /* 3. Buffer far-end */
    if (WebRtcAecm_BufferFarend(aecm_inst, far_ref, FRAME_SIZE_SAMPLES) != 0) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ BufferFarend failed\n");
        return;
    }
    
    /* 4. Process AEC */
    int16_t aec_output[FRAME_SIZE_SAMPLES];
    if (WebRtcAecm_Process(aecm_inst, agc_frame, NULL, aec_output, 10) != 0) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ AEC process failed\n");
        memcpy(output, agc_frame, FRAME_SIZE_BYTES);
        return;
    }
    
    memcpy(output, aec_output, FRAME_SIZE_BYTES);
    
    /* 5. Calculate ERLE metrics */
    float near_power = 0.0f, out_power = 0.0f;
    for (int i = 0; i < FRAME_SIZE_SAMPLES; i++) {
        float n = (float)near_mic[i] / 32768.0f;
        float o = (float)aec_output[i] / 32768.0f;
        near_power += n * n;
        out_power += o * o;
    }
    near_power /= FRAME_SIZE_SAMPLES;
    out_power /= FRAME_SIZE_SAMPLES;
    
    if (out_power > 1e-10f) {
        float frame_erle = 10.0f * log10f((near_power + 1e-10f) / out_power);
        
        erle_metrics.frame_erle_db = frame_erle;
        if (frame_erle < erle_metrics.min_erle_db) 
            erle_metrics.min_erle_db = frame_erle;
        if (frame_erle > erle_metrics.max_erle_db) 
            erle_metrics.max_erle_db = frame_erle;
        
        erle_metrics.frame_count++;
        erle_metrics.avg_erle_db = (erle_metrics.avg_erle_db * (erle_metrics.frame_count - 1) + frame_erle) 
                                   / erle_metrics.frame_count;
    }
    
    dwt_stop(&dwt_ts);
}

ErleMetrics_t* aecm_get_metrics(void) {
    return &erle_metrics;
}

void aecm_free(void) {
    if (aecm_inst) {
        WebRtcAecm_Free(aecm_inst);
        aecm_inst = NULL;
    }
}

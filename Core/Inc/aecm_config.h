#ifndef __AECM_CONFIG_H__
#define __AECM_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"

/* ====== AUDIO PARAMETERS ====== */
#define SAMPLE_RATE_HZ          8000
#define FRAME_SIZE_SAMPLES      160      /* 20ms @ 8kHz */
#define BYTES_PER_SAMPLE        2        /* 16-bit */
#define FRAME_SIZE_BYTES        (FRAME_SIZE_SAMPLES * BYTES_PER_SAMPLE)
#define NUM_CHANNELS            2        /* Stereo: L/R */

/* ====== AECM MODES ====== */
#define AECM_MODE_COMFORT       0
#define AECM_MODE_MOBILE        2
#define AECM_MODE_AGGRESSIVE    4
#define AECM_MODE_CURRENT       AECM_MODE_MOBILE

/* ====== BUFFER CONFIGURATION ====== */
#define AECM_BUFFER_SIZE        (FRAME_SIZE_SAMPLES * 4)  /* 80ms history */
#define FAR_END_BUFFER_SIZE     (FRAME_SIZE_SAMPLES * 10) /* 200ms history */
#define RTP_BUFFER_FRAMES       16
#define JITTER_BUFFER_SIZE      (FRAME_SIZE_SAMPLES * 32) /* 640ms @ 8kHz */

/* ====== DMA CONFIGURATION ====== */
#define DMA_SAI_RX_PRIORITY     DMA_PRIORITY_HIGH
#define DMA_SAI_TX_PRIORITY     DMA_PRIORITY_HIGH

/* ====== ERLE METRICS ====== */
typedef struct {
    float frame_erle_db;
    float min_erle_db;
    float max_erle_db;
    float avg_erle_db;
    uint32_t frame_count;
    uint32_t skipped_frames;
} ErleMetrics_t;

/* ====== AGC PARAMETERS ====== */
#define AGC_TARGET_LEVEL_DBM0   -20     /* -20 dBm0 */
#define AGC_MAX_GAIN_DB         30
#define AGC_MIN_GAIN_DB         0
#define AGC_UPDATE_PERIOD_MS    100

/* ====== JITTER BUFFER ====== */
#define JITTER_BUFFER_MIN_MS    20
#define JITTER_BUFFER_MAX_MS    200
#define JITTER_AUTO_TUNE_ENABLE 1

/* ====== RTP CONFIGURATION ====== */
#define RTP_PAYLOAD_TYPE_G711A  8
#define RTP_CLOCK_RATE          8000
#define RTP_SSRC                0x12345678
#define RTP_UDP_PORT            16384

/* ====== DWT DELAY MEASUREMENT ====== */
#define DWT_ENABLE              1
#define DWT_FREQ_MHZ            520

/* ====== ITM TELEMETRY ====== */
#define ITM_CHANNEL_ERLE        0
#define ITM_CHANNEL_AGC         1
#define ITM_CHANNEL_RTP         2
#define ITM_CHANNEL_DEBUG       31

/* ====== NetX DUO CONFIGURATION ====== */
#define NETX_IP_ADDRESS         IP_ADDRESS(192, 168, 1, 100)
#define NETX_NETMASK            IP_ADDRESS(255, 255, 255, 0)
#define NETX_GATEWAY            IP_ADDRESS(192, 168, 1, 1)
#define NETX_REMOTE_IP          IP_ADDRESS(192, 168, 1, 101)
#define NETX_REMOTE_PORT        16384

#endif /* __AECM_CONFIG_H__ */

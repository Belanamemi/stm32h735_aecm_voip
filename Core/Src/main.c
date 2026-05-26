#include "main.h"
#include "aecm_processor.h"
#include "voip_rtp.h"
#include "jitter_buffer.h"
#include "dwt_delay_measurement.h"
#include "itm_telemetry.h"
#include "voip_codec.h"
#include "wm8994_codec.h"
#include <string.h>

/* ====== GLOBAL HANDLERS ====== */
I2C_HandleTypeDef hi2c1;
SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;
ETH_HandleTypeDef heth;
DMA_HandleTypeDef hdma_sai1_a;
DMA_HandleTypeDef hdma_sai1_b;

/* ====== AUDIO BUFFERS ====== */
static int16_t sai_rx_buffer[FRAME_SIZE_SAMPLES * NUM_CHANNELS] __attribute__((section(".aec_buffers")));
static int16_t sai_tx_buffer[FRAME_SIZE_SAMPLES * NUM_CHANNELS] __attribute__((section(".aec_buffers")));
static int16_t aec_output[FRAME_SIZE_SAMPLES] __attribute__((section(".aec_buffers")));
static int16_t far_ref_buffer[FRAME_SIZE_SAMPLES] __attribute__((section(".aec_buffers")));

/* ====== JITTER BUFFER ====== */
static JitterBuffer_t jitter_buf __attribute__((section(".rtp_buffers")));

/* ====== RTP STATE ====== */
static uint16_t rtp_seq = 0;
static uint32_t rtp_ts = 0;

/* ====== FRAME SYNCHRONIZATION ====== */
static volatile uint32_t frame_ready = 0;
static uint32_t frame_count = 0;

/* ====== SAI DMA CALLBACKS ====== */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
    frame_ready = 1;
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai) {
    /* TX complete - prepare next frame */
}

/**
 * @brief Main VoIP processing loop
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    
    /* Initialize subsystems */
    itm_init();
    dwt_init();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_SAI1_Init();
    MX_ETH_Init();
    MX_NVIC_Init();
    
    itm_printf(ITM_CHANNEL_DEBUG, "\n══════════════════════════════════════════════════════\n");
    itm_printf(ITM_CHANNEL_DEBUG, "🎤 STM32H735 WebRTC AECM VoIP System v1.0\n");
    itm_printf(ITM_CHANNEL_DEBUG, "══════════════════════════════════════════════════════\n\n");
    
    /* Initialize WM8994 stereo codec (Headphone + Microphone) */
    if (wm8994_init(&hi2c1) != 0) {
        itm_printf(ITM_CHANNEL_DEBUG, "❌ WM8994 codec initialization failed\n");
        while(1) { HAL_Delay(100); }
    }
    
    wm8994_set_input_gain(&hi2c1, 20);   /* Microphone gain +20dB */
    wm8994_set_output_gain(&hi2c1, 0);   /* Headphone gain 0dB */
    
    /* Start SAI DMA (async) */
    HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *)sai_rx_buffer, FRAME_SIZE_SAMPLES * NUM_CHANNELS);
    HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t *)sai_tx_buffer, FRAME_SIZE_SAMPLES * NUM_CHANNELS);
    
    /* Initialize AECM */
    aecm_init();
    jitter_buffer_init(&jitter_buf);
    rtp_init();
    
    itm_printf(ITM_CHANNEL_DEBUG, "✅ All subsystems initialized\n\n");
    
    /* ====== MAIN LOOP ====== */
    while (1) {
        if (!frame_ready) continue;
        frame_ready = 0;
        
        DwtTimestamp_t dwt_ts;
        dwt_start(&dwt_ts);
        
        /* Extract mono from stereo RX (left channel) */
        int16_t near_mic_mono[FRAME_SIZE_SAMPLES];
        for (int i = 0; i < FRAME_SIZE_SAMPLES; i++) {
            near_mic_mono[i] = sai_rx_buffer[i * NUM_CHANNELS];
        }
        
        /* Receive RTP packet from remote peer */
        RtpPacket_t rtp_pkt;
        if (rtp_recv(&rtp_pkt) == 0) {
            /* Decode G.711 A-law to PCM16 */
            g711_alaw_decode(rtp_pkt.payload, far_ref_buffer, rtp_pkt.payload_len);
            jitter_buffer_push(&jitter_buf, far_ref_buffer, FRAME_SIZE_SAMPLES);
            jitter_buffer_auto_tune(&jitter_buf);
        }
        
        /* Pop from jitter buffer for playback sync */
        if (jitter_buffer_pop(&jitter_buf, far_ref_buffer, FRAME_SIZE_SAMPLES) != 0) {
            memset(far_ref_buffer, 0, FRAME_SIZE_BYTES);
        }
        
        /* Process AECM + AGC */
        aecm_process_frame(near_mic_mono, far_ref_buffer, aec_output);
        
        /* Playback AEC output (mono -> stereo L/R) */
        for (int i = 0; i < FRAME_SIZE_SAMPLES; i++) {
            sai_tx_buffer[i * NUM_CHANNELS] = aec_output[i];      /* Left */
            sai_tx_buffer[i * NUM_CHANNELS + 1] = aec_output[i];  /* Right */
        }
        
        /* Encode AEC output to G.711 A-law */
        uint8_t encoded_payload[FRAME_SIZE_SAMPLES];
        g711_alaw_encode(aec_output, encoded_payload, FRAME_SIZE_SAMPLES);
        
        /* Send RTP packet to remote peer */
        RtpPacket_t tx_pkt;
        rtp_create_header(&tx_pkt, rtp_seq++, rtp_ts);
        rtp_ts += FRAME_SIZE_SAMPLES;
        memcpy(tx_pkt.payload, encoded_payload, FRAME_SIZE_SAMPLES);
        tx_pkt.payload_len = FRAME_SIZE_SAMPLES;
        rtp_send(&tx_pkt, "192.168.1.101", RTP_UDP_PORT);
        
        dwt_stop(&dwt_ts);
        
        /* Telemetry every 10 frames (200ms) */
        if (++frame_count % 10 == 0) {
            ErleMetrics_t *metrics = aecm_get_metrics();
            JitterStats_t jitter_stats;
            jitter_buffer_get_stats(&jitter_buf, &jitter_stats);
            
            HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            
            itm_printf(ITM_CHANNEL_DEBUG,
                "Frame %lu | ERLE: %.1f dB | Jitter: %.1f ms | Latency: %.2f ms\n",
                frame_count, metrics->avg_erle_db, jitter_stats.jitter_ms, dwt_ts.elapsed_ms);
        }
    }
    
    aecm_free();
    return 0;
}

/**
 * @brief System Clock Configuration @ 520 MHz
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    
    /* Configure HSE and PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;     /* 25 MHz HSE / 5 = 5 MHz */
    RCC_OscInitStruct.PLL.PLLN = 104;   /* 5 MHz * 104 = 520 MHz */
    RCC_OscInitStruct.PLL.PLLP = 1;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    RCC_OscInitStruct.PLL.PLLR = 2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    /* Configure system clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                   RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
    
    /* Configure SAI clock (from PLL, divided for 8kHz) */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
    PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_D2PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = LED_GREEN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);
}

void MX_I2C1_Init(void) {
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x10707CBB;  /* 100 kHz @ 520 MHz */
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
}

void MX_SAI1_Init(void) {
    /* SAI1 Block A: RX (Microphone CN11) */
    hsai_BlockA1.Instance = SAI1_Block_A;
    hsai_BlockA1.Init.AudioMode = SAI_MODESLAVE_RX;
    hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
    hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
    hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLED;
    hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF;
    hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
    hsai_BlockA1.Init.ActiveFrameLength = 32;
    hsai_BlockA1.Init.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
    hsai_BlockA1.Init.FSPolarity = SAI_FS_ACTIVE_LOW;
    hsai_BlockA1.Init.FSOffset = SAI_FS_BEFOREFIRSTBIT;
    hsai_BlockA1.Init.FirstBitOffset = 0;
    hsai_BlockA1.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
    hsai_BlockA1.Init.TraceBufferMode = SAI_TRACEBUFFER_DISABLE;
    
    /* SAI1 Block B: TX (Headphone HP1L/R) */
    hsai_BlockB1.Instance = SAI1_Block_B;
    hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_TX;
    hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS_EXT_SAI1;
    hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
    hsai_BlockB1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLED;
    hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF;
    hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
    hsai_BlockB1.Init.ActiveFrameLength = 32;
    hsai_BlockB1.Init.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
    hsai_BlockB1.Init.FSPolarity = SAI_FS_ACTIVE_LOW;
    hsai_BlockB1.Init.FSOffset = SAI_FS_BEFOREFIRSTBIT;
    hsai_BlockB1.Init.FirstBitOffset = 0;
    hsai_BlockB1.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
    hsai_BlockB1.Init.TraceBufferMode = SAI_TRACEBUFFER_DISABLE;
    
    HAL_SAI_Init(&hsai_BlockA1);
    HAL_SAI_Init(&hsai_BlockB1);
}

void MX_DMA_Init(void) {
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    /* SAI1 RX DMA */
    hdma_sai1_a.Instance = DMA2_Stream0;
    hdma_sai1_a.Init.Request = DMA_REQUEST_SAI1_A;
    hdma_sai1_a.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sai1_a.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sai1_a.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sai1_a.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sai1_a.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sai1_a.Init.Mode = DMA_CIRCULAR;
    hdma_sai1_a.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_sai1_a);
    __HAL_LINKDMA(&hsai_BlockA1, hdmarx, hdma_sai1_a);
    
    /* SAI1 TX DMA */
    hdma_sai1_b.Instance = DMA2_Stream1;
    hdma_sai1_b.Init.Request = DMA_REQUEST_SAI1_B;
    hdma_sai1_b.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sai1_b.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sai1_b.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sai1_b.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sai1_b.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sai1_b.Init.Mode = DMA_CIRCULAR;
    hdma_sai1_b.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_sai1_b);
    __HAL_LINKDMA(&hsai_BlockB1, hdmatx, hdma_sai1_b);
}

void MX_ETH_Init(void) {
    heth.Instance = ETH;
    heth.Init.MACAddr[0] = 0x00;
    heth.Init.MACAddr[1] = 0x11;
    heth.Init.MACAddr[2] = 0x22;
    heth.Init.MACAddr[3] = 0x33;
    heth.Init.MACAddr[4] = 0x44;
    heth.Init.MACAddr[5] = 0x55;
    /* Additional Ethernet config */
}

void MX_NVIC_Init(void) {
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}

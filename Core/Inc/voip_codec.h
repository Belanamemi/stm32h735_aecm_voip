#ifndef __VOIP_CODEC_H__
#define __VOIP_CODEC_H__

#include <stdint.h>
#include "aecm_config.h"

/* ====== G.711 A-LAW CODEC ====== */

/**
 * @brief Encode PCM16 to G.711 A-law
 * @param pcm16: Input 16-bit PCM samples
 * @param output: Output A-law bytes (8-bit)
 * @param len: Number of PCM samples
 */
void g711_alaw_encode(const int16_t *pcm16, uint8_t *output, uint32_t len);

/**
 * @brief Decode G.711 A-law to PCM16
 * @param alaw: Input A-law bytes (8-bit)
 * @param output: Output 16-bit PCM samples
 * @param len: Number of A-law bytes
 */
void g711_alaw_decode(const uint8_t *alaw, int16_t *output, uint32_t len);

#endif /* __VOIP_CODEC_H__ */

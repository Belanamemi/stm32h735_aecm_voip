#ifndef VOIP_CODEC_G711_H
#define VOIP_CODEC_G711_H

#include <stdint.h>

void g711_alaw_encode(const int16_t *pcm, uint8_t *alaw, uint16_t len);
void g711_alaw_decode(const uint8_t *alaw, int16_t *pcm, uint16_t len);

#endif

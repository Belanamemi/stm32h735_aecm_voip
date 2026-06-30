#ifndef AGC_PREPROCESSOR_H
#define AGC_PREPROCESSOR_H

#include <stdint.h>

void agc_init(void);
void agc_process(int16_t *frame, uint16_t len);

#endif

#ifndef pit_h
#define pit_h

#include "MKL05Z4.h"                   //Device header
void PitInitialize(void);
void PIT_IRQHandler(void);
int8_t two_taps(void);
void toogle_clear(void);
#endif

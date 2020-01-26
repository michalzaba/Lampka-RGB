#ifndef leds_h
#define leds_h

#include "MKL05Z4.h"                   //Device header

void ledsInitialize(void);

void ledsOff (void);
void ledsOn (void);
void ledGreenOff (void);
void ledGreenOn (void);
void ledRedOn (void);
void ledBlueOn (void);
int32_t buttonRead(void);
#endif

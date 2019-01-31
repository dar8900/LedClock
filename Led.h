#ifndef LED_H
#define LED_H

#include <Arduino.h>

void LedInit(void);
void ShowDateTimeDisplayBySensor(void);
void ShowDateTimeDisplayByButton(void);
void MinuteLed(uint8_t WichLed);
void CheckForDisplayTime(void);
#endif
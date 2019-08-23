#ifndef LED_H
#define LED_H

#include <Arduino.h>

void LedInit(void);
void BlinkLed(uint8_t Pin);
void ShowDateTimeDisplayBySensor(void);
void ShowDateTimeDisplayByButton(void);
void MinuteLed(uint8_t WichLed);
void CheckForDisplayTime(void);
void RotateLed(uint16_t Delay, uint8_t Times);
#endif
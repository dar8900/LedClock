#ifndef TIME_H
#define TIME_H

#include "RTClib.h"
extern DateTime GlobalTimeDate; 
extern uint8_t TimeNumbers[4]; 
extern uint8_t DateNumbers[4]; 

void RtcInit(void);
void GetTimeDate(void);
void SetTimeDate(void);
void CheckForSetTime(void);

void CheckForSetTimeDBG(void);
#endif
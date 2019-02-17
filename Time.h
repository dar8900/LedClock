#ifndef TIME_H
#define TIME_H

#include "RTClib.h"

#define SECOND_IN_MINUTE	 			  60
#define SECOND_IN_HOUR	   				3600
#define MINUTE_IN_XHOUR(XHour)	(XHour * 60)


extern DateTime GlobalTimeDate; 
extern uint8_t TimeNumbers[4]; 
extern uint8_t DateNumbers[4]; 
extern uint8_t TimerNumbers[4];
extern uint16_t MinuteTimer;
extern uint16_t MinuteToSecond;

void RtcInit(void);
void GetTimeDate(void);
void SetTimeDate(void);
void CheckForSetTime(void);
void SetTimer(void);
void StartTimer(void);

void CheckForSetTimeDBG(void);
#endif
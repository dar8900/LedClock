#ifndef SEGMENT_DISPLAY_H
#define SEGMENT_DISPLAY_H
#include <Arduino.h>

#define POINTS_ON 	true
#define POINTS_OFF 	false

void DisplaysInit(void);
void ClearDisplay(void);
void ShowNumber(uint8_t Numbers[], bool PointOn);
void BlinkErr(void);


#endif
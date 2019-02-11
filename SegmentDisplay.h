#ifndef SEGMENT_DISPLAY_H
#define SEGMENT_DISPLAY_H
#include <Arduino.h>

#define POINTS_ON 	true
#define POINTS_OFF 	false

#define FIRST_DIGIT  0x00
#define SECOND_DIGIT 0x01
#define THIRD_DIGIT  0x02
#define FOURTH_DIGIT 0x03


#define DIGIT_OFF	0x7f

/*
static int8_t TubeTab[] = 
{
	0x3f,  // 0
	0x06,  // 1
	0x5b,  // 2
	0x4f,  // 3
    0x66,  // 4
	0x6d,  // 5
	0x7d,  // 6
	0x07,  // 7
    0x7f,  // 8
	0x6f,  // 9
	0x77,  // A
	0x7c,  // b
    0x39,  // C
	0x5e,  // d
	0x79,  // E
	0x71,  // F
	0x76,  // H
};     
*/

void DisplaysInit(void);
void ClearDisplay(void);
void ClearSingleDigit(uint8_t Digit);
void ShowNumber(uint8_t Numbers[], bool PointOn);
void BlinkErr(void);


#endif
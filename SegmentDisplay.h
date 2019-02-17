#ifndef SEGMENT_DISPLAY_H
#define SEGMENT_DISPLAY_H
#include <Arduino.h>

#define POINTS_ON 	true
#define POINTS_OFF 	false

#define FIRST_DIGIT 	0
#define SECOND_DIGIT	1
#define THIRD_DIGIT 	2
#define FOURTH_DIGIT	3


#define DIGIT_OFF	0x7f

typedef enum
{
	NUMBER_0 = 0,
	NUMBER_1,
	NUMBER_2,
	NUMBER_3,
	NUMBER_4,
	NUMBER_5,
	NUMBER_6,
	NUMBER_7,
	NUMBER_8,
	NUMBER_9,
	MAX_NUMBERS
}DISPLAY_NUMBERS;


typedef enum
{
	LETTER_A = 10,
	LETTER_B,
	LETTER_C,
	LETTER_D,
	LETTER_E,
	LETTER_F,
	LETTER_H,
	LETTER_I,
	LETTER_L,
	LETTER_N,
	LETTER_O,
	LETTER_P,
	LETTER_R,
	LETTER_T,
	LETTER_U,
	EQUAL_SIGN,
	MAX_LETTER
}DISPLAY_LETTER;

void DisplaysInit(void);
void ClearDisplay(void);
void ClearPoint(void);
void ClearSingleDigit(uint8_t Digit);
void ShowNumber(uint8_t Numbers[], bool PointOn);
void ShowLetter(uint8_t Digit, uint8_t Letter);
void BlinkErr(void);
void CheckForSetBrightness(void);
void SetBrightness(void);
void BlinkWord(uint8_t FirstLetter, uint8_t SecondLetter, uint8_t ThirdLetter, uint8_t FourthLetter);

#endif
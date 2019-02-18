#include "SegmentDisplay.h"
#include "TM1637.h"
#include "Time.h"
#include "KeyBoard.h"
#include "LedClock.h"
#include "IrSensor.h"
#include <EEPROM.h>
#include "EepromAddr.h"

TM1637 HourDateDisplay(CLK_DISP_1,DIO_DISP_1);

static uint8_t Brightness = 0;
static uint8_t ErrorNumbr[4] = {0,0,0,0};
static uint8_t BrightNumbers[4] = {DIGIT_OFF, DIGIT_OFF, DIGIT_OFF, Brightness};

void DisplaysInit()
{
	uint8_t Brightness = 0;
	Brightness = EEPROM.read(BRIGHTNESS_ADDR);
	if(Brightness > 7)
		Brightness = 7;
	HourDateDisplay.set(Brightness, ADDR_AUTO, STARTADDR);
	HourDateDisplay.init();
	ShowLetter(FIRST_DIGIT,  LETTER_B);
	ShowLetter(SECOND_DIGIT, LETTER_R);
	ShowLetter(THIRD_DIGIT,  EQUAL_SIGN);
	ShowLetter(FOURTH_DIGIT, Brightness);
	delay(1500);
	ClearDisplay();
}

void ClearDisplay()
{
	HourDateDisplay.init();
	HourDateDisplay.point(POINTS_OFF);
}

void ClearSingleDigit(uint8_t Digit)
{
	HourDateDisplay.display(Digit, DIGIT_OFF);
}

void ShowNumber(uint8_t Numbers[], bool PointOn)
{
	HourDateDisplay.display(Numbers);
	if(PointOn == POINTS_ON)
		HourDateDisplay.point(POINTS_ON);
	else
		HourDateDisplay.point(POINTS_OFF);
}


void ShowLetter(uint8_t Digit, uint8_t Letter)
{
	HourDateDisplay.display(Digit, Letter);
}

void SetBrightness()
{
	static bool FirstEnter = true, ChangeBrightness = false;
	SettingBrightness = true;
	if(FirstEnter)
	{
		ShowLetter(FIRST_DIGIT,  LETTER_B);
		ShowLetter(SECOND_DIGIT, LETTER_R);
		ShowLetter(THIRD_DIGIT,  LETTER_I);
		ShowLetter(FOURTH_DIGIT, LETTER_T);
		OsDelay(1500);
		FirstEnter = false;
		Brightness = EEPROM.read(BRIGHTNESS_ADDR);
		BrightNumbers[3] = Brightness;
	}
	if(ChangeBrightness)
	{
		HourDateDisplay.set(Brightness, ADDR_AUTO, STARTADDR);
		ChangeBrightness = false;
	}
	ShowNumber(BrightNumbers, POINTS_OFF);
	switch(ButtonPress)
	{
		case UP:
			if(Brightness > 0)
				Brightness--;
			else
				Brightness = 7;
			BrightNumbers[3] = Brightness;
			ChangeBrightness = true;
			break;
		case DOWN:
			if(Brightness < 7)
				Brightness++;
			else
				Brightness = 0;
			BrightNumbers[3] = Brightness;
			ChangeBrightness = true;
			break;
		case OK:
			FirstEnter = true;
			HourDateDisplay.set(Brightness, ADDR_AUTO, STARTADDR);
			HourDateDisplay.init();
			EEPROM.update(BRIGHTNESS_ADDR, Brightness);
			SettingBrightness = false;
			ChangeBrightness = false;
			ShowLetter(FIRST_DIGIT, NUMBER_5);
			ShowLetter(SECOND_DIGIT, LETTER_E);
			ShowLetter(THIRD_DIGIT, LETTER_T);
			ClearSingleDigit(FOURTH_DIGIT);
			ButtonPress = NO_PRESS;
			OsDelay(1000);
			ClearDisplay();
			break;
		default:
			break;
	}
	ButtonPress = NO_PRESS;
	
}

void BlinkErr()
{
	for(uint8_t i = 0; i < 3; i++)
	{
		ShowLetter(FIRST_DIGIT, LETTER_E);
		ShowLetter(SECOND_DIGIT, LETTER_R);
		ShowLetter(THIRD_DIGIT, LETTER_R);
		ClearSingleDigit(FOURTH_DIGIT);
		delay(250);
		ClearDisplay();
		delay(250);
	}
}

void BlinkWord(uint8_t FirstLetter, uint8_t SecondLetter, uint8_t ThirdLetter, uint8_t FourthLetter)
{
	for(uint8_t i = 0; i < 3; i++)
	{
		if(FirstLetter != DIGIT_OFF)
			ShowLetter(FIRST_DIGIT, FirstLetter);
		else
			ClearSingleDigit(FirstLetter);
		if(SecondLetter != DIGIT_OFF)
			ShowLetter(SECOND_DIGIT, SecondLetter);
		else
			ClearSingleDigit(FirstLetter);
		if(ThirdLetter != DIGIT_OFF)
			ShowLetter(THIRD_DIGIT, ThirdLetter);
		else
			ClearSingleDigit(ThirdLetter);
		if(FourthLetter != DIGIT_OFF)
			ShowLetter(FOURTH_DIGIT, FourthLetter);
		else
			ClearSingleDigit(FourthLetter);
		delay(250);
		ClearDisplay();
		delay(250);
	}		 
}
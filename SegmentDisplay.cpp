#include "SegmentDisplay.h"
#include "TM1637.h"
#include "Time.h"
#include "KeyBoard.h"
#include "LedClock.h"
#include "IrSensor.h"
#include <EEPROM.h>
#include "EepromAddr.h"

TM1637 HourDate(CLK_DISP_1,DIO_DISP_1);

static uint8_t Brightness = 0;
static uint8_t ErrorNumbr[4] = {0,0,0,0};
static uint8_t BrightNumbers[4] = {DIGIT_OFF, DIGIT_OFF, DIGIT_OFF, Brightness};

void DisplaysInit()
{
	uint8_t Brightness = 0;
	Brightness = EEPROM.read(BRIGHTNESS_ADDR);
	if(Brightness > 7)
		Brightness = 7;
	HourDate.set(Brightness, ADDR_AUTO, STARTADDR);
	HourDate.init();
	ShowLetter(FIRST_DIGIT,  LETTER_B);
	ShowLetter(SECOND_DIGIT, LETTER_R);
	ShowLetter(THIRD_DIGIT,  EQUAL_SIGN);
	ShowLetter(FOURTH_DIGIT, Brightness);
	delay(1500);
	ClearDisplay();
}

void ClearDisplay()
{
	HourDate.init();
}

void ClearSingleDigit(uint8_t Digit)
{
	HourDate.display(Digit, DIGIT_OFF);
}

void ShowNumber(uint8_t Numbers[], bool PointOn)
{
	HourDate.display(Numbers);
	if(PointOn == POINTS_ON)
		HourDate.point(POINTS_ON);
	else
		HourDate.point(POINTS_OFF);
}


void ShowLetter(uint8_t Digit, uint8_t Letter)
{
	HourDate.display(Digit, Letter);
}

void CheckForSetBrightness()
{
	if(!SettingBrightness)
	{
		if(ButtonPress == DOWN)
		{
			SettingBrightness = true;
			ButtonPress = NO_PRESS;
		}
	}
}

void SetBrightness()
{
	static bool FirstEnter = true;
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
	ShowNumber(BrightNumbers, POINTS_OFF);
	switch(ButtonPress)
	{
		case UP:
			if(Brightness > 0)
				Brightness--;
			else
				Brightness = 7;
			BrightNumbers[3] = Brightness;
			break;
		case DOWN:
			if(Brightness < 7)
				Brightness++;
			else
				Brightness = 0;
			BrightNumbers[3] = Brightness;
			break;
		case OK:
			FirstEnter = true;
			HourDate.set(Brightness, ADDR_AUTO, STARTADDR);
			HourDate.init();
			EEPROM.update(BRIGHTNESS_ADDR, Brightness);
			SettingBrightness = false;
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
		ShowNumber(ErrorNumbr, true);
		OsDelay(500);
		ClearDisplay();
		ShowNumber(ErrorNumbr, false);
		OsDelay(500);
		ClearDisplay();
	}
}
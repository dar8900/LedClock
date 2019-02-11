#include "SegmentDisplay.h"
#include "TM1637.h"
#include "Time.h"
#include "KeyBoard.h"
#include "LedClock.h"
#include "IrSensor.h"

TM1637 HourDate(CLK_DISP_1,DIO_DISP_1);

static uint8_t ErrorNumbr[4] = {0,0,0,0};

void DisplaysInit()
{
	HourDate.set();
	HourDate.init();
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
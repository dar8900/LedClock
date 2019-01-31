#include "Led.h"
#include "Time.h"
#include "LedClock.h"
#include "SegmentDisplay.h"
#include "KeyBoard.h"

static void PilotMultiplexer(uint8_t State_1, uint8_t State_2, uint8_t State_3, uint8_t State_4)
{
	digitalWrite(MULTI_1, State_1);
	digitalWrite(MULTI_2, State_2);
	digitalWrite(MULTI_3, State_3);
	digitalWrite(MULTI_4, State_4);	
}

void LedInit()
{
	uint8_t LedIndex = 0;
	for(LedIndex = 0; LedIndex <= 11; LedIndex++)
	{
		MinuteLed(5 * LedIndex);
		OsDelay(300);
	}
}


void ShowDateTimeDisplayBySensor()
{
	if(ShowTimeDate)
	{
		bool TogglePoint = POINTS_ON;
		for(uint8_t i = 0; i < 8; i++)
		{
			ShowNumber(TimeNumbers, TogglePoint);
			TogglePoint = !TogglePoint;
			OsDelay(1000);			
		}
		ShowNumber(DateNumbers, POINTS_OFF);
		OsDelay(6000);
		ShowTimeDate = false;
	}
}

void CheckForDisplayTime()
{
	if(!SettingTime)
	{
		if(ButtonPress == UP && SensorOn)
		{
			SensorOn = false;
		}
		else if(ButtonPress == UP && !SensorOn)
		{
			SensorOn = true;	
		}
	}
}

void ShowDateTimeDisplayByButton()
{
	bool TogglePoint = POINTS_ON;
	for(uint8_t i = 0; i < 8; i++)
	{
		ShowNumber(TimeNumbers, TogglePoint);
		TogglePoint = !TogglePoint;
		OsDelay(1000);			
	}
	ShowNumber(DateNumbers, POINTS_OFF);
	OsDelay(6000);
}

void MinuteLed(uint8_t WichLed)
{
	switch(WichLed)
	{
		case 0:
			PilotMultiplexer(LOW, LOW, LOW, LOW);
			break;
		case 5:
			PilotMultiplexer(HIGH, LOW, LOW, LOW);
			break;
		case 10:
			PilotMultiplexer(LOW, HIGH, LOW, LOW);
			break;
		case 15:
			PilotMultiplexer(HIGH, HIGH, LOW, LOW);
			break;
		case 20:
			PilotMultiplexer(LOW, LOW, HIGH, LOW);
			break;
		case 25:
			PilotMultiplexer(HIGH, LOW, HIGH, LOW);
			break;
		case 30:
			PilotMultiplexer(LOW, HIGH, HIGH, LOW);
			break;
		case 35:
			PilotMultiplexer(HIGH, HIGH, HIGH, LOW);
			break;
		case 40:
			PilotMultiplexer(LOW, LOW, LOW, HIGH);
			break;
		case 45:
			PilotMultiplexer(HIGH, LOW, LOW, HIGH);
			break;
		case 50:
			PilotMultiplexer(LOW, HIGH, LOW, HIGH);
			break;
		case 55:
			PilotMultiplexer(HIGH, HIGH, LOW, HIGH);
			break;
		default:
			break;				
	}
}
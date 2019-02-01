#include "Led.h"
#include "Time.h"
#include "LedClock.h"
#include "SegmentDisplay.h"
#include "KeyBoard.h"

typedef struct 
{
	uint8_t Bit1;
	uint8_t Bit2;
	uint8_t Bit3;
	uint8_t Bit4;
}MULTIPLEXER_VAR;

typedef enum
{
	CHANNEL_0 = 0,
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4,
	CHANNEL_5,
	CHANNEL_6,
	CHANNEL_7,
	CHANNEL_8,
	CHANNEL_9,
	CHANNEL_10,
	CHANNEL_11,
	CHANNEL_12,
	CHANNEL_13,
	CHANNEL_14,
	CHANNEL_15,
	MAX_CHANNEL
}MULTIPLEXER_CHANNEL;

static MULTIPLEXER_VAR MultiplexerChannel[MAX_CHANNEL] = 
{
	{LOW,  LOW,  LOW,  LOW},
	{HIGH, LOW,  LOW,  LOW},
	{LOW,  HIGH, LOW,  LOW},
	{HIGH, HIGH, LOW,  LOW},
	{LOW,  LOW,  HIGH, LOW},
	{HIGH, LOW,  HIGH, LOW},
	{LOW,  HIGH, HIGH, LOW},
	{HIGH, HIGH, HIGH, LOW},
	{LOW,  LOW,  LOW,  HIGH},
	{HIGH, LOW,  LOW,  HIGH},
	{LOW,  HIGH, LOW,  HIGH},
	{HIGH, HIGH, LOW,  HIGH},
	{LOW,  LOW,  HIGH, HIGH},
	{HIGH, LOW,  HIGH, HIGH},
	{LOW,  HIGH, HIGH, HIGH},
	{HIGH, HIGH, HIGH, HIGH},
};


static void PilotMultiplexer(MULTIPLEXER_VAR MultiState)
{
	digitalWrite(MULTI_1, MultiState.Bit1);
	digitalWrite(MULTI_2, MultiState.Bit2);
	digitalWrite(MULTI_3, MultiState.Bit3);
	digitalWrite(MULTI_4, MultiState.Bit4);	
}

static void RotateLed(uint16_t Delay)
{
	uint8_t LedIndex = 0;
	if(!DisableLed)
	{
		for(LedIndex = 0; LedIndex <= 11; LedIndex++)
		{
			MinuteLed(5 * LedIndex);
			OsDelay(Delay);
		}
	}
}


void LedInit()
{
	digitalWrite(ENABLE_MUX, LOW);
	DisableLed = false;
	RotateLed(300);
}


void ShowDateTimeDisplayBySensor()
{
	if(ShowTimeDate)
	{
		bool TogglePoint = POINTS_ON;
		RotateLed(80);
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
	if(!DisableLed)
	{
		switch(WichLed)
		{
			case 0:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_0]);
				break;
			case 5:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_1]);
				break;
			case 10:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_2]);
				break;
			case 15:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_3]);
				break;
			case 20:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_4]);
				break;
			case 25:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_5]);
				break;
			case 30:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_6]);
				break;
			case 35:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_7]);
				break;
			case 40:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_8]);
				break;
			case 45:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_9]);
				break;
			case 50:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_10]);
				break;
			case 55:
				PilotMultiplexer(MultiplexerChannel[CHANNEL_11]);
				break;
			default:
				break;				
		}
	} 
}
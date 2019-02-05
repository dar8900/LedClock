#include "Led.h"
#include "Time.h"
#include "LedClock.h"
#include "SegmentDisplay.h"
#include "KeyBoard.h"

typedef struct 
{
	byte ForAnd;
	byte ForOr;
	bool IsOn;
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



// static MULTIPLEXER_VAR MultiplexerChannel[MAX_CHANNEL] = 
// {
	// {0,  0,  0,  0},
	// {1,  0,  0,  0},
	// {0,  1,  0,  0},
	// {1,  1,  0,  0},
	// {0,  0,  1,  0},
	// {1,  0,  1,  0},
	// {0,  1,  1,  0},
	// {1,  1,  1,  0},
	// {0,  0,  0,  1},
	// {1,  0,  0,  1},
	// {0,  1,  0,  1},
	// {1,  1,  0,  1},
	// {0,  0,  1,  1},
	// {1,  0,  1,  1},
	// {0,  1,  1,  1},
	// {1,  1,  1,  1},
// };

static MULTIPLEXER_VAR MultiplexerTab[MAX_CHANNEL] = 
{
	{B11000011, B00000000, false},
	{B11000111, B00000100, false},
	{B11001011, B00001000, false},
	{B11001111, B00001100, false},
	{B11010011, B00010000, false},
	{B11010111, B00010100, false},
	{B11011011, B00011000, false},
	{B11011111, B00011100, false},
	{B11100011, B00100000, false},
	{B11100111, B00100100, false},
	{B11101011, B00101000, false},
	{B11101111, B00101100, false},
	{B11110011, B00110000, false},
	{B11110111, B00110100, false},
	{B11111011, B00111000, false},
	{B11111111, B00111100, false},	
};

static void ToggleChannelFlagOff(uint8_t StillOnChannel)
{
	uint8_t ChannelIndex = CHANNEL_0;
	for(ChannelIndex = CHANNEL_0; ChannelIndex < CHANNEL_12; ChannelIndex++)
	{
		if(ChannelIndex == StillOnChannel)
			continue;
		if(MultiplexerTab[ChannelIndex].IsOn)
			MultiplexerTab[ChannelIndex].IsOn = false;
	}
}


static void PilotMultiplexer(uint8_t ChannelIndex)
{
	PORTD = (PORTD & MultiplexerTab[ChannelIndex].ForAnd) | MultiplexerTab[ChannelIndex].ForOr;
	MultiplexerTab[ChannelIndex].IsOn = true;
}

void RotateLed(uint16_t Delay)
{
	uint8_t ChannelIndex = CHANNEL_0;
	if(!DisableLed)
	{
		for(ChannelIndex = CHANNEL_0; ChannelIndex < CHANNEL_12; ChannelIndex++)
		{
			PilotMultiplexer(ChannelIndex);
			OsDelay(Delay);
		}
	}
}

void LedInit()
{
	digitalWrite(ENABLE_MUX, LOW);
	DisableLed = false;
	RotateLed(50);
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
		ClearDisplay();
		ShowNumber(DateNumbers, POINTS_OFF);
		OsDelay(6000);
		ShowTimeDate = false;
		ClearDisplay();
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
	ClearDisplay();
	ShowNumber(DateNumbers, POINTS_OFF);
	OsDelay(6000);
	ClearDisplay();
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


void MinuteLed(uint8_t WichLed)
{
	uint8_t ChannelIndex = CHANNEL_0;
	if(!DisableLed)
	{
		switch(WichLed)
		{
			case 0:
				ChannelIndex = CHANNEL_0;
				break;
			case 5:
				ChannelIndex = CHANNEL_1;
				break;
			case 10:
				ChannelIndex = CHANNEL_2;
				break;
			case 15:
				ChannelIndex = CHANNEL_3;
				break;
			case 20:
				ChannelIndex = CHANNEL_4;
				break;
			case 25:
				ChannelIndex = CHANNEL_5;
				break;
			case 30:
				ChannelIndex = CHANNEL_6;
				break;
			case 35:
				ChannelIndex = CHANNEL_7;
				break;
			case 40:
				ChannelIndex = CHANNEL_8;
				break;
			case 45:
				ChannelIndex = CHANNEL_9;
				break;
			case 50:
				ChannelIndex = CHANNEL_10;
				break;
			case 55:
				ChannelIndex = CHANNEL_11;
				break;
			default:
				ChannelIndex = MAX_CHANNEL;
				break;				
		}
		if(ChannelIndex != MAX_CHANNEL)
		{
			if(!MultiplexerTab[ChannelIndex].IsOn)
				PilotMultiplexer(ChannelIndex);
			ToggleChannelFlagOff(ChannelIndex);
		}
	} 
}
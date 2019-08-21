#include "Led.h"
#include "Time.h"
#include "LedClock.h"
#include "SegmentDisplay.h"
#include "KeyBoard.h"

#define MAX_MINUTE 12

typedef struct 
{
	uint8_t State_1;
	uint8_t State_2;
	uint8_t State_3;
	uint8_t State_4;
	bool IsOn;
}MULTIPLEXER_VAR;

typedef struct
{
	uint8_t Minute;
	uint8_t Channel;
}MINUTE_TO_CHANNEL_S;


typedef enum
{
	CHANNEL_0 = 0,            //    {0,  0,  0,  0, false},
	CHANNEL_1,                //    {1,  0,  0,  0, false},
	CHANNEL_2,                //    {0,  1,  0,  0, false},
	CHANNEL_3,                //    {1,  1,  0,  0, false},
	CHANNEL_4,                //    {0,  0,  1,  0, false},
	CHANNEL_5,                //    {1,  0,  1,  0, false},
	CHANNEL_6,                //    {0,  1,  1,  0, false},
	CHANNEL_7,                //    {1,  1,  1,  0, false},
	CHANNEL_8,                //    {0,  0,  0,  1, false},
	CHANNEL_9,                //    {1,  0,  0,  1, false},
	CHANNEL_10,               //    {0,  1,  0,  1, false},
	CHANNEL_11,               //    {1,  1,  0,  1, false},
	CHANNEL_12,               //    {0,  0,  1,  1, false},
	CHANNEL_13,               //    {1,  0,  1,  1, false},
	CHANNEL_14,               //    {0,  1,  1,  1, false},
	CHANNEL_15,               //    {1,  1,  1,  1, false},
	MAX_CHANNEL
}MULTIPLEXER_CHANNEL;


static bool IsLedRotating = false;

static MULTIPLEXER_VAR MultiplexerTab[MAX_CHANNEL] = 
{
	{0,  0,  0,  0, false},
	{1,  0,  0,  0, false},
	{0,  1,  0,  0, false},
	{1,  1,  0,  0, false},
	{0,  0,  1,  0, false},
	{1,  0,  1,  0, false},
	{0,  1,  1,  0, false},
	{1,  1,  1,  0, false},
	{0,  0,  0,  1, false},
	{1,  0,  0,  1, false},
	{0,  1,  0,  1, false},
	{1,  1,  0,  1, false},
	{0,  0,  1,  1, false},
	{1,  0,  1,  1, false},
	{0,  1,  1,  1, false},
	{1,  1,  1,  1, false},
};

static const MINUTE_TO_CHANNEL_S MinuteToChannel[MAX_MINUTE] = 
{
	{0 , CHANNEL_5 },
	{5 , CHANNEL_4 },
	{10, CHANNEL_3 },
	{15, CHANNEL_2 },
	{20, CHANNEL_1 },
	{25, CHANNEL_0 },
	{30, CHANNEL_11},
	{35, CHANNEL_10},
	{40, CHANNEL_9 },
	{45, CHANNEL_8 },
	{50, CHANNEL_7 },
	{55, CHANNEL_6 },
};

// static MULTIPLEXER_VAR MultiplexerTab[MAX_CHANNEL] = 
// {
	// {B11000011, B00000000, false},
	// {B11000111, B00000100, false},
	// {B11001011, B00001000, false},
	// {B11001111, B00001100, false},
	// {B11010011, B00010000, false},
	// {B11010111, B00010100, false},
	// {B11011011, B00011000, false},
	// {B11011111, B00011100, false},
	// {B11100011, B00100000, false},
	// {B11100111, B00100100, false},
	// {B11101011, B00101000, false},
	// {B11101111, B00101100, false},
	// {B11110011, B00110000, false},
	// {B11110111, B00110100, false},
	// {B11111011, B00111000, false},
	// {B11111111, B00111100, false},	
// };

void BlinkLed(uint8_t Pin)
{
	digitalWrite(Pin, HIGH);
	delay(50);
	digitalWrite(Pin, LOW);
	delay(50);
}

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
	// PORTD = (PORTD & MultiplexerTab[ChannelIndex].ForAnd) | MultiplexerTab[ChannelIndex].ForOr;
	digitalWrite(MULTI_1, MultiplexerTab[ChannelIndex].State_1);
	digitalWrite(MULTI_2, MultiplexerTab[ChannelIndex].State_2);
	digitalWrite(MULTI_3, MultiplexerTab[ChannelIndex].State_3);
	digitalWrite(MULTI_4, MultiplexerTab[ChannelIndex].State_4);
	
	MultiplexerTab[ChannelIndex].IsOn = true;
}

void RotateLed(uint16_t Delay)
{
	uint8_t ChannelIndex = CHANNEL_0;
	if(!DisableLed)
	{
		IsLedRotating = true;
		for(ChannelIndex = CHANNEL_0; ChannelIndex < CHANNEL_12; ChannelIndex++)
		{
			PilotMultiplexer(ChannelIndex);
			delay(Delay);
		}
		IsLedRotating = false;
	}
}

void LedInit()
{
	uint8_t RollNumb[4] = {0,0,0,0};
	uint8_t NumbToRoll = 0;
	digitalWrite(ENABLE_MUX, LOW);
	DisableLed = false;
	RotateLed(500);
	for(NumbToRoll = 0; NumbToRoll < 10; NumbToRoll++)
	{
		ShowNumber(RollNumb, true);
		RollNumb[0] = NumbToRoll;
		RollNumb[1] = NumbToRoll;
		RollNumb[2] = NumbToRoll;
		RollNumb[3] = NumbToRoll;
		delay(100);
	}
	for(NumbToRoll = 0; NumbToRoll < 3; NumbToRoll++)
	{
		ShowNumber(RollNumb, false);
		delay(50);
		ClearDisplay();
		delay(50);
	}
}


void ShowDateTimeDisplayBySensor()
{
	if(ShowTimeDate)
	{
		bool TogglePoint = POINTS_ON;
		//RotateLed(80);
		for(uint8_t i = 0; i < 8; i++)
		{
			ShowNumber(TimeNumbers, TogglePoint);
			TogglePoint = !TogglePoint;
			OsDelay(500);			
		}
		ClearDisplay();
		ShowNumber(DateNumbers, POINTS_OFF);
		OsDelay(3000);
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
		OsDelay(500);			
	}
	ClearDisplay();
	ShowNumber(DateNumbers, POINTS_OFF);
	OsDelay(3000);
	ClearDisplay();
}

// void MinuteLed(uint8_t WichLed)
// {
	// uint8_t ChannelIndex = CHANNEL_0;
	// if(!DisableLed && !IsLedRotating)
	// {
		// switch(WichLed)
		// {
			// case 0:
				// ChannelIndex = CHANNEL_0;
				// break;
			// case 5:
				// ChannelIndex = CHANNEL_1;
				// break;
			// case 10:
				// ChannelIndex = CHANNEL_2;
				// break;
			// case 15:
				// ChannelIndex = CHANNEL_3;
				// break;
			// case 20:
				// ChannelIndex = CHANNEL_4;
				// break;
			// case 25:
				// ChannelIndex = CHANNEL_5;
				// break;
			// case 30:
				// ChannelIndex = CHANNEL_6;
				// break;
			// case 35:
				// ChannelIndex = CHANNEL_7;
				// break;
			// case 40:
				// ChannelIndex = CHANNEL_8;
				// break;
			// case 45:
				// ChannelIndex = CHANNEL_9;
				// break;
			// case 50:
				// ChannelIndex = CHANNEL_10;
				// break;
			// case 55:
				// ChannelIndex = CHANNEL_11;
				// break;
			// default:
				// ChannelIndex = MAX_CHANNEL;
				// break;				
		// }
		// if(ChannelIndex != MAX_CHANNEL)
		// {
			// if(!MultiplexerTab[ChannelIndex].IsOn)
				// PilotMultiplexer(ChannelIndex);
			// ToggleChannelFlagOff(ChannelIndex);
		// }
	// } 
// }

void MinuteLed(uint8_t WichLed)
{
	if(WichLed % 5 == 0)
	{
		uint8_t ChannelIndex = MinuteToChannel[WichLed / 5].Channel;
		if(!DisableLed && !IsLedRotating)
		{
			if(!MultiplexerTab[ChannelIndex].IsOn)
				PilotMultiplexer(ChannelIndex);
			ToggleChannelFlagOff(ChannelIndex);			
		}
	}
}

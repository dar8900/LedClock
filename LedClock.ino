#include "LedClock.h"
#include "Led.h"
#include "IrSensor.h"
#include "SegmentDisplay.h"
#include "KeyBoard.h"
#include "Time.h"
#include <EEPROM.h>
#include "EepromAddr.h"
#include <avr/io.h>
#include <avr/wdt.h>

#define Reset() wdt_enable(WDTO_30MS); while(1) {} 

bool ShowTimeDate;
bool SensorOn;
bool SettingTime;
// bool ResetArduino;
bool DisableLed;
bool SettingBrightness;
bool SettingTimer;
bool StartingTimer;
bool TimerIsSet;
bool ShowAlwaysTimeDisplay;

uint8_t ResetNumbers[4];

void Led( void *pvParameters );
void GesEvents( void *pvParameters );
void ReadSensor(void *pvParameters);
void GetTime(void *pvParameters);
void KeyBoard(void *pvParameters);

#ifdef TASK_TEST
void TaskTest( void *pvParameters );
#endif

//declare reset function @ address 0
uint16_t ResetNumb;

void setup()
{
	Serial.begin(9600);
	pinMode(MULTI_1, OUTPUT);
	pinMode(MULTI_2, OUTPUT);
	pinMode(MULTI_3, OUTPUT);
	pinMode(MULTI_4, OUTPUT);
	pinMode(ENABLE_MUX, OUTPUT);
	pinMode(UP_BUTTON,   INPUT);
	pinMode(DOWN_BUTTON, INPUT);
	pinMode(OK_BUTTON,   INPUT);
	// pinMode(SENSOR_INPUT,   INPUT);
	
	DisplaysInit();
	RtcInit();
	LedInit();
	
	
#ifdef TASK_LED	
	xTaskCreate(
	Led
	,  (const portCHAR *)"Led"   // A name just for humans
	,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
#endif

#ifdef TASK_GES_EVENTS	
	xTaskCreate(
	GesEvents
	,  (const portCHAR *) "GesEvents"
	,  128  // Stack size
	,  NULL
	,  3  // Priority
	,  NULL );
#endif

#ifdef TASK_SENSOR
	xTaskCreate(
	ReadSensor
	,  (const portCHAR *) "Sensor"
	,  128  // Stack size
	,  NULL
	,  2  // Priority
	,  NULL );
#endif

#ifdef TASK_TIME	
	xTaskCreate(
	GetTime
	,  (const portCHAR *) "GetTime"
	,  64  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
#endif

#ifdef TASK_KEYBOARD	
	xTaskCreate(
	KeyBoard
	,  (const portCHAR *) "KeyBoard"
	,  64  // Stack size
	,  NULL
	,  3  // Priority
	,  NULL );
#endif

#ifdef TASK_TEST	
	xTaskCreate(
	TaskTest
	,  (const portCHAR *) "Test"
	,  64  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
#endif	
	
	// Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.

}

void loop()
{
	
	
}

#ifdef TASK_LED
void Led(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint16_t Cnt = 0;
	bool TogglePoint = false;
	for(;;)
	{
		if(!SettingTime && !SettingBrightness && !SettingTimer && !StartingTimer)
		{	
			if(!ShowAlwaysTimeDisplay)
			{
				if(SensorOn)
					ShowDateTimeDisplayBySensor();
				else
				{
					Cnt++;
					if(Cnt == 5000)
					{
						ShowDateTimeDisplayByButton();
						Cnt = 0;
					}
				}
			}
			else
			{
				Cnt++;
				ShowNumber(TimeNumbers, TogglePoint);
				if(Cnt >= 5)
				{
					TogglePoint = !TogglePoint;
					Cnt = 0;
				}
			}
			// MinuteLed(GlobalTimeDate.minute()); 
			MinuteLed(GlobalTimeDate.second());  /* DBG */
		}
		else if(SettingTime)
		{
			SetTimeDate();
		}
		else if(SettingBrightness)
		{
			SetBrightness();
		}
		else if(SettingTimer)
		{
			SetTimer();
		}
		else if(StartingTimer)
		{
			StartTimer();
			// MinuteLed(GlobalTimeDate.minute()); 
			MinuteLed(GlobalTimeDate.second());  /* DBG */
		}
		OsDelay(100);
	}
}
#endif

#ifdef TASK_GES_EVENTS
void GesEvents(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	ButtonPress = NO_PRESS;
	for(;;)
	{
		if(!SettingTime && !SettingBrightness && !SettingTimer && !StartingTimer)
		{
			ListenSensor();
			switch(ButtonPress)
			{
				case UP:
					if(SensorOn)
					{
						SensorOn = false;
						BlinkWord(NUMBER_8, LETTER_U, LETTER_T, LETTER_N);
					}
					else
					{
						SensorOn = true;
						BlinkWord(NUMBER_5, LETTER_E, LETTER_N, NUMBER_5);
					}
					break;
				case DOWN:
					BlinkWord(LETTER_T, LETTER_N, LETTER_R, DIGIT_OFF);
					SettingTimer = true;
					break;
				case OK:
					if(TimerIsSet)
					{
						ClearDisplay();
						BlinkWord(NUMBER_5, LETTER_T, LETTER_R, LETTER_T);
						StartingTimer = true;
					}
					else
					{
						ClearDisplay();
						BlinkWord(NUMBER_5, LETTER_E, LETTER_T, DIGIT_OFF);
						BlinkWord(LETTER_T, LETTER_N, LETTER_R, DIGIT_OFF);
					}
					break;
				case LONG_UP:
					if(!ShowAlwaysTimeDisplay)
					{
						ShowAlwaysTimeDisplay = true;
						BlinkWord(LETTER_T, LETTER_I, LETTER_N, LETTER_E);
					}
					else
					{
						ClearDisplay();
						ShowAlwaysTimeDisplay = false;
						OsDelay(1000);
					}
					break;
				case LONG_DOWN:
					SettingBrightness = true;						
					break;
				case LONG_OK:
					SettingTime = true;
					break;
				default:
					break;
			}
			ButtonPress = NO_PRESS;
		}
		else if(StartingTimer)
		{
			if(ButtonPress == OK)
			{
				ClearDisplay();
				BlinkWord(NUMBER_5, LETTER_T, NUMBER_0, LETTER_P);
				MinuteTimer = 0;
				MinuteToSecond = 0;	
				ButtonPress = NO_PRESS;
				StartingTimer = false;
				TimerIsSet = false;	
			}	
		}
		if(DisableLed)
		{
			digitalWrite(ENABLE_MUX, HIGH);
		}
		OsDelay(100);
	}
}
#endif

#ifdef TASK_SENSOR
void ReadSensor(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	for(;;)
	{
		if(!SettingTime && !SettingBrightness && !SettingTimer && !StartingTimer)
			ListenSensor();
		OsDelay(200);
	}
}
#endif

#ifdef TASK_TIME
void GetTime(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	for(;;)
	{
		if(!SettingTime)
		{
			GetTimeDate();
		}
		OsDelay(500);
	}
}
#endif

#ifdef TASK_KEYBOARD
void KeyBoard(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	for(;;)
	{
		CheckButtons();
		OsDelay(25);
	}
}
#endif

#ifdef TASK_TEST
void TaskTest(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	uint8_t Cnt = 5;
	bool Toggle = true;
	for(;;)
	{
		CheckForSetTimeDBG();
		if(SettingTime)
			SetTimeDate();
		MinuteLed(GlobalTimeDate.second());
		ShowNumber(TimeNumbers, Toggle);
		Cnt--;
		if(Cnt == 0)
		{
			Toggle = !Toggle;
			Cnt = 5;
		}
		OsDelay(100);
	}
}
#endif
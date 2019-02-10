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
bool ResetArduino;
bool DisableLed;

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

static void EepromInit()
{
	uint8_t ResetValue = 0;
	ResetValue = EEPROM.read(RESET_ADDR);
	EEPROM.get(RESET_NUMB_ADDR, ResetNumb);
	if(ResetValue != 0 || ResetNumb != 0)
	{
		if(ResetValue != 0xff)
		{
			ResetArduino = true;
			EEPROM.update(RESET_ADDR, 0);
			// ResetNumbers[0] = ResetNumb / 1000;
			// ResetNumbers[1] = (ResetNumb % 1000) / 100;
			// ResetNumbers[2] = ((ResetNumb % 1000) % 100) / 10 ;
			// ResetNumbers[3] = (((ResetNumb % 1000) % 100) % 10);
			// ShowNumber(ResetNumbers, false);
			// Serial.println(ResetNumb);
			// OsDelay(3000);
			ResetArduino = false;
			
		}
		else
		{
			Serial.println("First go");
			EEPROM.put(RESET_NUMB_ADDR, 0);
		}
	}
}

static void CheckForReset()
{
	if(ButtonPress == DOWN && !SettingTime)
	{
		EEPROM.update(RESET_ADDR, 1);
		ResetNumb++;
		EEPROM.put(RESET_NUMB_ADDR, ResetNumb);
		ButtonPress == NO_PRESS;	
		// digitalWrite(NOT_USED_1, LOW);
	}	
}

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
	
	// pinMode(NOT_USED_1, OUTPUT);
	// digitalWrite(NOT_USED_1, HIGH);
	
	DisplaysInit();
	RtcInit();
	
	
	
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
	uint16_t Cnt = 100;
	LedInit();
	for(;;)
	{
		if(!SettingTime && !ResetArduino)
		{
			Cnt--;
			if(Cnt == 0)
			{
				Cnt = 100;
				if(SensorOn)
					ShowDateTimeDisplayBySensor();
				else
					ShowDateTimeDisplayByButton();
				// MinuteLed(GlobalTimeDate.minute()); /* DBG */
			}
			MinuteLed(GlobalTimeDate.second());
		}
		else
		{
			SetTimeDate();
		}
		OsDelay(100);
	}
}
#endif

#ifdef TASK_GES_EVENTS
void GesEvents(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	EepromInit();
	for(;;)
	{
		CheckForReset();
		CheckForSetTime();
		CheckForDisplayTime();
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
		if(!SettingTime && !ResetArduino)
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
		OsDelay(50);
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
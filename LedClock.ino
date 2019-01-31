#include "LedClock.h"
#include "Led.h"
#include "IrSensor.h"
#include "SegmentDisplay.h"
#include "KeyBoard.h"
#include "Time.h"

bool ShowTimeDate;
bool SensorOn;
bool SettingTime;
bool ResetArduino;

void Led( void *pvParameters );
void GesEvents( void *pvParameters );
void ReadSensor(void *pvParameters);
void GetTime(void *pvParameters);
void KeyBoard(void *pvParameters);

//declare reset function @ address 0
void(* Reset) (void) = 0;

void setup()
{
	Serial.begin(9600);
	pinMode(MULTI_1, OUTPUT);
	pinMode(MULTI_2, OUTPUT);
	pinMode(MULTI_3, OUTPUT);
	pinMode(MULTI_4, OUTPUT);
	pinMode(UP_BUTTON,   INPUT);
	pinMode(DOWN_BUTTON, INPUT);
	pinMode(OK_BUTTON,   INPUT);	
	
#ifdef TASK_LED	
	xTaskCreate(
	Led
	,  (const portCHAR *)"Led"   // A name just for humans
	,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
#endif

#ifdef TASK_GES_EVENTS	
	xTaskCreate(
	GesEvents
	,  (const portCHAR *) "GesEvents"
	,  128  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
#endif

#ifdef TASK_SENSOR
	xTaskCreate(
	ReadSensor
	,  (const portCHAR *) "Sensor"
	,  128  // Stack size
	,  NULL
	,  1  // Priority
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
	LedInit();
	for(;;)
	{
		if(!SettingTime)
		{
			if(SensorOn)
				ShowDateTimeDisplayBySensor();
			else
				ShowDateTimeDisplayByButton();
			MinuteLed(GlobalTimeDate.minute());
		}
		else
		{
			SetTimeDate();
		}
		OsDelay(500);
	}
}
#endif

#ifdef TASK_GES_EVENTS
void GesEvents(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	for(;;)
	{
		if(ResetArduino)
		{
			ResetArduino = false;
			Reset();
		}
		
		CheckForSetTime();
		CheckForDisplayTime();
		OsDelay(50);
	}
}
#endif

#ifdef TASK_SENSOR
void ReadSensor(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	for(;;)
	{
		if(!SettingTime)
			ListenSensor();
		OsDelay(200);
	}
}
#endif

#ifdef TASK_TIME
void GetTime(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	RtcInit();
	for(;;)
	{
		if(!SettingTime)
			GetTimeDate();
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
		OsDelay(20);
	}
}
#endif
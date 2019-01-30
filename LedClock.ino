#include <Arduino_FreeRTOS.h>
#include "Led.h"
#include "IrSensor.h"
#include "SegmentDisplay.h"
#include "KeyBoard.h"

void Led( void *pvParameters );
void SegDiplay( void *pvParameters );
void ReadSensor(void *pvParameters);
void GetTime(void *pvParameters);
void KeyBoard(void *pvParameters);



void setup()
{
	Serial.begin(9600);
	
	xTaskCreate(
	Led
	,  (const portCHAR *)"Led"   // A name just for humans
	,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	SegDiplay
	,  (const portCHAR *) "SegDiplay"
	,  128  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );

	xTaskCreate(
	SegDiplay
	,  (const portCHAR *) "Sensor"
	,  128  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
	
	xTaskCreate(
	GetTime
	,  (const portCHAR *) "Sensor"
	,  64  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
	
	xTaskCreate(
	KeyBoard
	,  (const portCHAR *) "Sensor"
	,  64  // Stack size
	,  NULL
	,  1  // Priority
	,  NULL );
	// Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.

}

void loop()
{
	
	
}

void Led(void *pvParameters)  // This is a task.
{
	//vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
	(void) pvParameters;
	for(;;)
	{
		
	}
}

void SegDiplay(void *pvParameters)  // This is a task.
{
	//vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
	(void) pvParameters;
	for(;;)
	{
		
	}
}


void ReadSensor(void *pvParameters)  // This is a task.
{
	//vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
	(void) pvParameters;
	for(;;)
	{
		
	}
}

void GetTime(void *pvParameters)  // This is a task.
{
	//vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
	(void) pvParameters;
	for(;;)
	{
		
	}
}

void KeyBoard(void *pvParameters)  // This is a task.
{
	//vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
	(void) pvParameters;
	for(;;)
	{
		CheckButtons();
	}
}
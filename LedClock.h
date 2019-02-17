#ifndef LEDCLOK_H
#define LEDCLOK_H
#include <Arduino_FreeRTOS.h>
#include "TaskOptions.h"

#define OsDelay(ms) vTaskDelay(ms/portTICK_PERIOD_MS)



extern bool ShowTimeDate;
extern bool SensorOn;
extern bool SettingTime;
// extern bool ResetArduino;
extern bool DisableLed;
extern bool SettingBrightness;
extern bool SettingTimer;
extern bool TimerIsSet;

typedef enum
{
	MULTI_1     =  2,
	MULTI_2     =  3,
	MULTI_3     =  4,
	MULTI_4     =  5,
	CLK_DISP_1  =  6,  
	DIO_DISP_1  =  7,  
	NOT_USED_1  =  8,  // Not Used
	NOT_USED_2  =  9,  // Not Used
	UP_BUTTON   = 10,
	DOWN_BUTTON = 11,
	OK_BUTTON   = 12,
	ENABLE_MUX  = 13,
	MAX_PIN
	
}PIN_CONF;



#endif
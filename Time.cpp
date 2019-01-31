#include "Time.h"
#include "KeyBoard.h"
#include "SegmentDisplay.h"
#include "LedClock.h"

static RTC_DS1307 rtc;
DateTime GlobalTimeDate;
uint8_t TimeNumbers[4]; 
uint8_t DateNumbers[4]; 

static uint8_t AdjustTime[4] = {0, 0, 0 ,0};

void RtcInit()
{
	if (! rtc.begin()) 
	{
		while(1)
		{
			BlinkErr();
			ResetArduino = true;
		}
	}
	if (! rtc.isrunning())
	{
		SensorOn = false;
		BlinkErr();
		SetTimeDate();
		SensorOn = true;
	}
}

void GetTimeDate()
{
	GlobalTimeDate = rtc.now();
	TimeNumbers[0] = GlobalTimeDate.hour() / 10;
	TimeNumbers[1] = GlobalTimeDate.hour() % 10;
	TimeNumbers[2] = GlobalTimeDate.minute() / 10;
	TimeNumbers[3] = GlobalTimeDate.minute() % 10;
	
	DateNumbers[0] = GlobalTimeDate.day() / 10;
	DateNumbers[1] = GlobalTimeDate.day() % 10;
	DateNumbers[2] = GlobalTimeDate.month() / 10;
	DateNumbers[3] = GlobalTimeDate.month() % 10;
}

void CheckForSetTime()
{
	if(ButtonPress == OK)
	{
		if(!SettingTime)
			SettingTime = true;
	}
}

void SetTimeDate()
{
	bool IsTime = true, Exit = false, IsHour = true, IsDay = true;
	uint8_t hour = 0, minute = 0, second = 0, day = 0, month = 0;
	SettingTime = true;
	while(!Exit)
	{
		ShowNumber(AdjustTime, true);
		switch(ButtonPress)
		{
			case UP:
				if(IsTime)
				{
					if(IsHour)
					{
						if(hour > 0)
							hour--;
						else
							hour = 23;
						AdjustTime[0] = hour / 10;
						AdjustTime[1] = hour % 10;
					}
						
					else
					{
						if(minute > 0)
							minute--;
						else
							minute = 59;
						AdjustTime[2] = minute / 10;
						AdjustTime[3] = minute % 10;
					}
				}
				else
				{
					if(IsDay)
					{
						if(day > 1)
							day--;
						else
							day = 31;
						AdjustTime[0] = day / 10;
						AdjustTime[1] = day % 10;
					}
						
					else
					{
						if(month > 1)
							month--;
						else
							month = 12;
						AdjustTime[2] = month / 10;
						AdjustTime[3] = month % 10;
					}						
				}
				break;
			case DOWN:
				if(IsTime)
				{
					if(IsHour)
					{
						if(hour < 23)
							hour++;
						else
							hour = 0;
						AdjustTime[0] = hour / 10;
						AdjustTime[1] = hour % 10;
					}
						
					else
					{
						if(minute < 59)
							minute++;
						else
							minute = 0;
						AdjustTime[2] = minute / 10;
						AdjustTime[3] = minute % 10;
					}
				}
				else
				{
					if(IsDay)
					{
						if(day < 31)
							day++;
						else
							day = 1;
						AdjustTime[0] = day / 10;
						AdjustTime[1] = day % 10;
					}
						
					else
					{
						if(month < 12)
							month++;
						else
							month = 1;
						AdjustTime[2] = month / 10;
						AdjustTime[3] = month % 10;
					}						
				}
				break;
			case OK:
				if(IsTime)
				{
					if(IsHour)
						IsHour = false;
					else
						IsTime = false;
				}
				else
				{
					if(IsDay)
						IsDay = false;
					else
						Exit = true;
				}
				break;
			default:
				break;
		}
		OsDelay(100);
	}
	rtc.adjust(DateTime(2019, month, day, hour, minute, 0)); // anno, mese, giorno, 3h ,0m,0s
	SettingTime = false;
}

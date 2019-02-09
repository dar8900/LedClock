#include "Time.h"
#include "KeyBoard.h"
#include "SegmentDisplay.h"
#include "LedClock.h"
#include "Led.h"

static RTC_DS1307 rtc;
DateTime GlobalTimeDate;
uint8_t TimeNumbers[4]; 
uint8_t DateNumbers[4]; 

static uint8_t AdjustTime[4] = {9, 9, 9, 9};

static uint8_t DayInMonth[] = 
{
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31
};

void RtcInit()
{
	if (! rtc.begin()) 
	{
		uint8_t Cnt = 3;
		while(Cnt)
		{
			BlinkErr();
			OsDelay(500);
			Cnt--;
			// ResetArduino = true;
		}
	}
	if (! rtc.isrunning())
	{
		// Serial.println("Setting Time");
		SensorOn = false;
		BlinkErr();
		// SetTimeDate();
		// SensorOn = true;
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
	if(!SettingTime)
	{
		if(ButtonPress == OK)
		{
			SettingTime = true;
			ButtonPress = NO_PRESS;
		}
	}
}

void CheckForSetTimeDBG()
{
	if(!SettingTime)
	{
		if(ButtonPress == OK)
		{
			SettingTime = true;
			ButtonPress = NO_PRESS;
		}
	}
}

void SetTimeDate()
{ 
	static bool IsTime = true;
	static bool IsHour = true, IsMonth = true, IsDay = false, RefreshDisplay = true;
	static uint8_t hour = 0, minute = 0, second = 0, day = 0, month = 0;
	static uint16_t year = 2019;
	SettingTime = true;
	
	if(IsTime)
	{
		if(RefreshDisplay)
		{
			ShowNumber(AdjustTime, true);
			RefreshDisplay = false;
		}
		// if(IsHour)
			// MinuteLed(hour);
		// else
			// MinuteLed(minute);
	}
	else
	{
		if(RefreshDisplay)
		{
			ShowNumber(AdjustTime, false);
			RefreshDisplay = false;
		}
		// RotateLed(10);
	}
	switch(ButtonPress)
	{
		case UP:
			BlinkLed(MULTI_2);
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
				if(IsMonth)
				{
					if(month > 1)
						month--;
					else
						month = 12;
					AdjustTime[2] = month / 10;
					AdjustTime[3] = month % 10;
				}
				else if(IsDay)
				{
					if(day > 1)
						day--;
					else
						day = DayInMonth[month-1];
					AdjustTime[0] = day / 10;
					AdjustTime[1] = day % 10;
				}
				else
				{
					if(year > 2019)
						year--;
					else
						year = 2099;
					AdjustTime[0] = year / 1000;
					AdjustTime[1] = 0;
					AdjustTime[2] = (year % 100) / 10;
					AdjustTime[3] = (year % 100) % 10;						
				}
			}
			RefreshDisplay = true;
			break;
		case DOWN:
			BlinkLed(MULTI_2);
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
				if(IsMonth)
				{
					if(month < 12)
						month++;
					else
						month = 1;
					AdjustTime[2] = month / 10;
					AdjustTime[3] = month % 10;
				}	
				else if(IsDay)
				{
					if(day < DayInMonth[month-1])
						day++;
					else
						day = 1;
					AdjustTime[0] = day / 10;
					AdjustTime[1] = day % 10;
				}	
				else
				{
					if(year < 2099)
						year++;
					else
						year = 2019;
					AdjustTime[0] = year / 1000;
					AdjustTime[1] = 0;
					AdjustTime[2] = (year % 100) / 10;
					AdjustTime[3] = (year % 100) % 10;	
				}
			}
			RefreshDisplay = true;
			break;
		case OK:
			BlinkLed(MULTI_2);
			if(IsTime)
			{
				if(IsHour)
				{
					IsHour = false;
					RefreshDisplay = true;
					// RotateLed(100);
				}
				else
				{
					IsTime = false;
					IsHour = true;
					AdjustTime[0] = 0;
					AdjustTime[1] = 1;
					AdjustTime[2] = 0;
					AdjustTime[3] = 1;
					RefreshDisplay = true;
					// RotateLed(100);
					// RotateLed(100);
				}
			}
			else
			{
				if(IsMonth)
				{
					IsMonth = false;
					IsDay = true;
					RefreshDisplay = true;
					// RotateLed(100);
				}
				else if(IsDay)
				{
					IsDay = false;
					RefreshDisplay = true;
					AdjustTime[0] = 2;
					AdjustTime[1] = 0;
					AdjustTime[2] = 1;
					AdjustTime[3] = 9;
					// RotateLed(100);						
				}
				else
				{
					IsTime = true;
					IsHour = true; 
					IsMonth = true;
					IsDay = false;
					rtc.adjust(DateTime(2019, month, day, hour, minute, 0)); // anno, mese, giorno, 3h ,0m,0s
					hour = 0;
					minute = 0;
					second = 0;
					day = 0;
					month = 0;
					year = 2019;
					ClearDisplay();
					SettingTime = false;
					ButtonPress = NO_PRESS;
					// RotateLed(100);
					// RotateLed(100);
				}
			}
			break;
		default:
			break;
	}
}

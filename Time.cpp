#include "Time.h"
#include "KeyBoard.h"
#include "SegmentDisplay.h"
#include "LedClock.h"
#include "Led.h"

static RTC_DS1307 rtc;
DateTime GlobalTimeDate;
uint8_t TimeNumbers[4]; 
uint8_t DateNumbers[4]; 
uint8_t TimerNumbers[4] = {DIGIT_OFF, DIGIT_OFF, 0, 0};
uint16_t MinuteTimer;
uint16_t MinuteToSecond;
static uint8_t NewSecond, OldSecond;
bool SecondTick;

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

static bool IsTime = true;
static bool IsHour = true, IsMonth = true, IsDay = false, RefreshDisplay = true;
static uint8_t hour = 0, minute = 0, second = 0, day = 0, month = 0;
static uint16_t year = 2019;

void RtcInit()
{
	if (! rtc.begin()) 
	{
		while(1)
		{
			BlinkErr();
			delay(500);
		}
	}
	if (! rtc.isrunning())
	{
		// Serial.println("Setting Time");
		SensorOn = false;
		BlinkErr();
		SettingTime = true;
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
	NewSecond = GlobalTimeDate.second();
	if(NewSecond != OldSecond)
	{
		SecondTick = true;
		OldSecond = NewSecond;
	}
	
	if(GlobalTimeDate.day() < 10)
	{
		DateNumbers[0] = DIGIT_OFF;
		DateNumbers[1] = GlobalTimeDate.day() % 10;
	}
	else
	{
		DateNumbers[0] = GlobalTimeDate.day() / 10;
		DateNumbers[1] = GlobalTimeDate.day() % 10;
	}
	if(GlobalTimeDate.month() < 10)
	{
		DateNumbers[2] = DIGIT_OFF;
		DateNumbers[3] = GlobalTimeDate.month() % 10;
	}
	else
	{
		DateNumbers[2] = GlobalTimeDate.month() / 10;
		DateNumbers[3] = GlobalTimeDate.month() % 10;
	}
}

void SetTimeDate()
{ 
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
	ButtonPress = NO_PRESS;
}


void SetTimer()
{
	static bool RefreshDisplay = true;
	ShowNumber(TimerNumbers, false);
	
	switch(ButtonPress)
	{
		case UP:
			if(MinuteTimer > 0)
				MinuteTimer--;
			else
				MinuteTimer = 99;
			RefreshDisplay = true;
			break;
		case DOWN:
			if(MinuteTimer < 99)
				MinuteTimer++;
			else
				MinuteTimer = 0;
			RefreshDisplay = true;
			break;
		case OK:
			RefreshDisplay = false;
			TimerIsSet = true;
			MinuteToSecond = MinuteTimer * SECOND_IN_MINUTE;
			SettingTimer = false;
			ClearDisplay();
			break;
		default:
			break;
	}
	if(RefreshDisplay)
	{
		TimerNumbers[0] = DIGIT_OFF;
		TimerNumbers[1] = DIGIT_OFF;
		TimerNumbers[2] = (MinuteTimer % 100) / 10;
		TimerNumbers[3] = (MinuteTimer % 100) % 10;
		RefreshDisplay = false;
	}
	ButtonPress = NO_PRESS;
}

void StartTimer()
{
	static bool StopTimer = false;
	if(SecondTick && MinuteToSecond > 0)
	{
		SecondTick = false;
		MinuteToSecond--;
		StopTimer = false;
	}
	if(MinuteToSecond == 0)
	{
		StopTimer = true;
		BlinkWord(LETTER_E, LETTER_N, LETTER_D, DIGIT_OFF);
	}
	if(!StopTimer)
	{
		if(MinuteTimer < 10)
		{
			TimerNumbers[0] = DIGIT_OFF;
			TimerNumbers[1] = MinuteToSecond / 60;
			TimerNumbers[2] = (MinuteToSecond % 60) / 10;
			TimerNumbers[3] = (MinuteToSecond % 60) % 10;
			ShowNumber(TimerNumbers, true);
		}
		else
		{
			TimerNumbers[0] = (MinuteToSecond / 60) / 10;
			TimerNumbers[1] = (MinuteToSecond / 60) % 10;
			TimerNumbers[2] = (MinuteToSecond % 60) / 10;
			TimerNumbers[3] = (MinuteToSecond % 60) % 10;
			ShowNumber(TimerNumbers, true);
		}
	}
}
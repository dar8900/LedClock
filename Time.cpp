#include "Time.h"
#include "KeyBoard.h"
#include "SegmentDisplay.h"


static RTC_DS1307 rtc;
DateTime GlobalTimeDate; 

void RtcInit()
{
	if (! rtc.begin()) 
	{
		Serial.println("Couldn't find RTC");
		while (1);
	}
	if (! rtc.isrunning())
	{
		Serial.println("RTC is NOT running!");
	}
}

void GetTimeDate()
{
	GlobalTimeDate = rtc.now();
}

// DateTime now = rtc.now();
// Serial.print(now.year(), DEC);
// Serial.print('/');
// Serial.print(now.month(), DEC);
// Serial.print('/');
// Serial.print(now.day(), DEC);
// Serial.print(" (");
// Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
// Serial.print(") ");
// Serial.print(now.hour(), DEC);
// Serial.print(':');
// Serial.print(now.minute(), DEC);
// Serial.print(':');
// Serial.print(now.second(), DEC);
// Serial.println();
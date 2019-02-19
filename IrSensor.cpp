#include "IrSensor.h"
#include "LedClock.h"

#define THR_SENSOR	512



void ListenSensor()
{
	int ReadSignal = 0;
	if(SensorOn)
	{
		ReadSignal = analogRead(A0);
		Serial.println(ReadSignal); // PER DBG
		// if(ReadSignal > THR_SENSOR && !ShowTimeDate)
		// {
			// ShowTimeDate = true;
		// }
	}
}
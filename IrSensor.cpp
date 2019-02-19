#include "IrSensor.h"
#include "LedClock.h"

#define THR_SENSOR	1000


void ListenSensor()
{
	int ReadSignal = 0;
	if(SensorOn)
	{
		ReadSignal = analogRead(A0);
		if(ReadSignal < THR_SENSOR && !ShowTimeDate)
		{
			ShowTimeDate = true;
		}
	}
}
#include "IrSensor.h"
#include "LedClock.h"

#define THR_SENSOR	800


void ListenSensor()
{
	int ReadSignal = 0;
	if(SensorOn)
	{
		ReadSignal = analogRead(A1);
		// Serial.println(ReadSignal);
		if(ReadSignal < THR_SENSOR && !ShowTimeDate)
		{
			// Serial.println(ReadSignal);
			ShowTimeDate = true;
		}
		// if(digitalRead(SENSOR_INPUT) && !ShowTimeDate)
		// {
			// Serial.println("Sensore attivo");
			// ShowTimeDate = true;
		// }
	}
}
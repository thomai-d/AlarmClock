#ifndef TrendData_H
#define TrendData_H

#include "SensorHardware.h"
#include "Config.h"
#include "Debug.h"

#define SAMPLE_YMAX		64		// Trend maximum height of a data point.

class TrendDataClass
{
public:

	void Initialize();

	void Service();

	// Index of the last inserted sample.
	uint8_t currentItem;

	uint8_t tempData[TREND_MAX_ITEMS];
	uint8_t pressureData[TREND_MAX_ITEMS];

	uint8_t PressureToY(float pressure);
	uint8_t TemperatureToY(float temp);

private:

	uint32_t lastService;

};

extern TrendDataClass TrendData;

#endif
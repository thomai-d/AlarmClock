#include "TrendData.h"

uint8_t trendBlur(uint8_t, uint8_t);

void TrendDataClass::Initialize()
{
	this->lastService = millis();
	this->currentItem = TREND_MAX_ITEMS - 1;

	// Fix: Discard first value.
	// Sometimes this value is not valid.
	SensorHardware.GetTemperature();
	SensorHardware.GetPressure();

	// Initialize trend data with current value.
	uint8_t temp = this->TemperatureToY(SensorHardware.GetTemperature());
	uint8_t pressure = this->PressureToY(SensorHardware.GetPressure());
	for (uint8_t n = 0; n < TREND_MAX_ITEMS; n++)
	{
		this->tempData[n] = trendBlur(temp, n);
		this->pressureData[n] = trendBlur(pressure, n);
	}
}

void TrendDataClass::Service()
{
	auto now = millis();
	if (now - this->lastService < SAMPLE_TIME_MS)
		return;

	this->currentItem = ++this->currentItem % TREND_MAX_ITEMS;
	this->tempData[this->currentItem] = this->TemperatureToY(SensorHardware.GetTemperature());
	this->pressureData[this->currentItem] = this->PressureToY(SensorHardware.GetPressure());
	this->lastService = now;
}

uint8_t TrendDataClass::PressureToY(float pressure)
{
	if (pressure < TREND_PRESSURE_MIN) return SAMPLE_YMAX;
	if (pressure > TREND_PRESSURE_MAX) return 0;
	return SAMPLE_YMAX - (uint8_t)((pressure - TREND_PRESSURE_MIN) / (TREND_PRESSURE_MAX - TREND_PRESSURE_MIN) * SAMPLE_YMAX);
}

uint8_t TrendDataClass::TemperatureToY(float temp)
{
	if (temp < TREND_TEMP_MIN) return SAMPLE_YMAX;
	if (temp > TREND_TEMP_MAX) return 0;
	return SAMPLE_YMAX - (uint8_t)((temp - TREND_TEMP_MIN) / (TREND_TEMP_MAX - TREND_TEMP_MIN) * SAMPLE_YMAX);
}

uint8_t trendBlur(uint8_t baseVal, uint8_t pos)
{
	return (uint8_t)min(max(0, baseVal + (5.0 * sin((float)(pos+1) / TREND_MAX_ITEMS * 20 * PI))), SAMPLE_YMAX);
}

TrendDataClass TrendData;
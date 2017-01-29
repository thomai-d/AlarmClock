#include "SensorHardware.h"

float SensorHardwareClass::GetTemperature()
{
	this->bmp180.begin();
	return this->bmp180.readTemperature();
}

float SensorHardwareClass::GetPressure()
{
	this->bmp180.begin();
	return (float)this->bmp180.readPressure() / 1000;
}

SensorHardwareClass SensorHardware;
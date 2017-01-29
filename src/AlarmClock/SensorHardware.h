#ifndef SENSOR_HARDWARE_H
#define SENSOR_HARDWARE_H

#include <Adafruit_BMP085.h>
#include <EEPROM.h>

class SensorHardwareClass
{
public:

	// Reads the temperature in °C.
	float GetTemperature();

	// Reads the pressure in kPa.
	float GetPressure();

private: 
	Adafruit_BMP085 bmp180;

};

extern SensorHardwareClass SensorHardware;

#endif


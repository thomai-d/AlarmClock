# AlarmClock
Arduino based alarm clock with wake up light

![WakeUpLight](https://github.com/thomai-d/AlarmClock/blob/master/img/PreAlarm.jpg)

## Features
* Two individually configurable alarm times
* 15 minutes before alarm, the power-LED is slowly ramped on
* LED wake up color is configurable by the user
* Slowly rising acoustic alarm
* Temperature and air pressure trend for the last 8 hours
* Display brightness is automatically reduced in the dark (LDR)
* Power fail safe (battery buffered RTC)

## Hardware
* atmega328 @ 16MHz
* 2.8" SPI TFT Screen (320x240) with ILI9331 controller
* DS3231 battery buffered RTC with I2C interface
* BMP180 temperature / pressure sensor with I2C interface
* High Power RGB-LED (~4.5W)
* Eagle files / part list included

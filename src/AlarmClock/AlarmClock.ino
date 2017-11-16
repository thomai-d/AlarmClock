#include <Wire.h>
#include <ucglib.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <DS3232RTC.h>
#include <Adafruit_BMP085.h>
#include <avr/wdt.h>
#include <color.h>

#include "Page.h"
#include "MainPage.h"
#include "ConfigPage.h"
#include "InputHardware.h"
#include "PinDefinitions.h"
#include "PageManager.h"
#include "UserActivityManager.h"
#include "TrendPage.h"
#include "TrendData.h"
#include "DateTime.h"
#include "Debug.h"

MainPage mainPage;
TrendPage trendPage;
ConfigPage configPage;

void setup() 
{
	Serial.begin(115200);
	Serial.println(F("AlarmClock"));
	delay(50);

	// Initialize watchdog.
	wdt_enable(WDTO_4S);

	// Initialize RTC.
	DateTime.Initialize();

	// Init display.
	Display2.Initialize();

	// Initialize stuff.
	AlarmManager.ReloadAlarms();
	InputHardware.Initialize();
	TrendData.Initialize();

	// Speaker test.
	OutputHardware.AlarmTest();

	// Initialize pages.
	PageManager.SetPage(0, &mainPage);
	PageManager.SetPage(1, &trendPage);
	PageManager.SetPage(2, &configPage);
	PageManager.Home();

	// Setup Timer2 (16ms)
	TCCR2A = 0;
	TCCR2B = 7;
	bitSet(TIMSK2, TOIE2);
}

void loop() 
{
	// Time can be synchronized from Serial.
	if (Serial.available())
	{
		if (Serial.peek() == 'T')
			DateTime.SyncFromSerial();
		else if (Serial.peek() == '!')
		{
			if (Serial.find("!Reset"))
			{
				AlarmManager.FactoryReset();
				Serial.println("Factory reset.");
			}
		}
	}
	
	AlarmManager.Service();
	TrendData.Service();
	UserActivityManager.Service();

	PageManager.Service();

	wdt_reset();
}

void OnButton2Down()
{
	if (AlarmManager.IsAlarmActive())
	{
		// Quit pending alarms.
		AlarmManager.QuitFirstAlarm();
	}
	else if (!Display2.IsBacklightOn())
	{
		// Wakeup.
		PageManager.Home();
	}
	else
	{
		// Next page.
		PageManager.NextPage();
	}
}

void OnAlarmActivated()
{
	PageManager.Home();
}

ISR(TIMER2_OVF_vect)
{
	cli();

	OutputHardware.OnTimer2Tick();
	Display2.OnTimer2Tick();

	sei();
}


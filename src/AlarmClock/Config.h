#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG

/* WORKAROUNDS */

#define ALTERNATIVE_BUTTONS	// for #1
#define ALTERNATIVE_ROTARY	// #1, #4, #5
//#define ALTERNATIVE_LDR		// #3
#define NO_LDR				// #1

/* FONTS */

#define FONT_BIG24	ucg_font_inr24_mn
#define FONT_MED14	ucg_font_helvR14_hr
#define FONT_TINY4	ucg_font_04b_03b_hn

#define BUTTON_DEBOUNCE_MS					100			// Pushbutton debounce time [ms].


/* BEHAVIOR */

#define USER_INACTIVITY_TIMEOUT_NORMAL		10			// Inactivity timeout for main page [s].
#define USER_INACTIVITY_TIMEOUT_INPUT		25			// Inactivity timeout when editing stuff [s].
#define USER_INACTIVITY_TIMEOUT_TREND		30			// Inactivity timeout when viewing trend [s].
#define USER_INACTIVITY_TIMEOUT_SETTINGS	60			// Inactivity timeout when viewing settings [s].

/* ALARM */

#define ALARM_STEPS_MINUTES					5			// Alarm change step [min].
#define ALARM_RAMP_SECONDS					15 * 60		// Time the light goes on before the alarm sound [s].

/* TREND */

#define TREND_PRESSURE_MIN					97			// Minumum pressure displayed by the trend [kPa].
#define TREND_PRESSURE_MAX					104			// Maximum pressure displayed by the trend [kPa].
#define TREND_PRESSURE_STEP					1			// Pressure grid line distance [kPa].

#define TREND_TEMP_MIN						14			// Minimum temperature displayed by the trend [°C].
#define TREND_TEMP_MAX						28			// Maximum temperature displayed by the trend [°C].
#define TREND_TEMP_STEP						2			// Temperature grid line distance [°K].

#define MINUTE_MS							60000			
#define SAMPLE_TIME_MS						5 * MINUTE_MS	// Delay between trend samples [ms].
#define TREND_MAX_ITEMS						100				// Trend maximum items.

/* DISPLAY */

#define LIGHT_SAMPLES				8					// Number of samples for averaging available light.
#define BACKLIGHT_MIN_LEVEL			3 					// Minimum backlight brighness.
#define BACKLIGHT_MAX_LEVEL			100 				// Maximum backlight brighness.
#define LED_ON_INTENSITY			255					// Intensity of LED light when manually activated.

// EEPROM Adresses
#define EEPROM_ALARM0_H		0
#define EEPROM_ALARM0_M		1
#define EEPROM_ALARM0_ON	2	
#define EEPROM_ALARM1_H		3
#define EEPROM_ALARM1_M		4
#define EEPROM_ALARM1_ON	5	
#define EEPROM_HUE			6				// Hue, [10°]
#define EEPROM_SAT			7				// Saturation, [1/10]


#endif

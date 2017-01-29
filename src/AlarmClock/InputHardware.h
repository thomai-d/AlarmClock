#ifndef INPUT_HARDWARE_H
#define INPUT_HARDWARE_H

/* 
	Rotary encoder + 2 pushbuttons using PCINT.

	C0 + C1		Rotary encoder
	C2 + C3		Two pushbuttons (low active)
*/

#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6
#define DIR_NONE	0x0
#define DIR_CW		0x10
#define DIR_CCW		0x20

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Time.h>

#include "Config.h"
#include "Display.h"
#include "Debug.h"
#include "PinDefinitions.h"
#include "IInputFocus.h"
#include "UserActivityManager.h"

#ifdef ALTERNATIVE_BUTTONS
#define BUTTON0_BIT 0
#define BUTTON1_BIT 3
#else
#define BUTTON0_BIT 3
#define BUTTON1_BIT 0
#endif


extern void OnButton2Down();

class InputHardwareClass
{
public:

	InputHardwareClass();

	// This method should be called when
	// the time is set.
	void Initialize();

	// This method should be invoked on PCINT1.
	// Optionally this method may be invoked cyclically (> 10ms)
	// to fix invalid states due to the debouncing algorithms
	// inability to handle short LOW-transients < debounce time.
	void OnPCINT1();

	// This method should be invoked on INT0.
	// Optionally this method may be invoked cyclically (> 10ms)
	// to fix invalid states due to the debouncing algorithms
	// inability to handle short LOW-transients < debounce time.
	void OnINT0();

	void BlockInput(bool blockModeButton = true) { this->blockInput = true; this->blockModeButton = blockModeButton; }
	
	void ActivateInput() { this->blockInput = false; this->blockModeButton = false; }

	// Gets the brightness value.
	uint8_t ReadBrightness();

	IInputFocus *Focus = 0;

	bool IsButton1Down() { return !bitRead(this->button_states, BUTTON1_BIT); }

private:

	// Rotary states.
	static const uint8_t lookupTable[7][4];
	uint8_t		rotary_state;

	// Button states.
	uint8_t		button_states = 0xff;
	uint32_t	b0_lastEvent;
	uint32_t	b1_lastEvent;
	uint32_t	b2_lastEvent;

	// Generic.
	volatile bool	blockInput;
	volatile bool	blockModeButton;
};

extern InputHardwareClass InputHardware;

#endif

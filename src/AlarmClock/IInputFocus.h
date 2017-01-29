#ifndef IINPUT_FOCUS
#define IINPUT_FOCUS

#include <inttypes.h>

/// Interface for an implementation that receives user input.
class IInputFocus
{
public:
	virtual void OnButton0Down() { };
	virtual void OnButton1Down() { };
	virtual void OnButton0Up(uint16_t downTimeMs) { };
	virtual void OnButton1Up(uint16_t downTimeMs) { };
	virtual void OnRotaryChanged(int8_t offset) { };
};

#endif
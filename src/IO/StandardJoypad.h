#pragma once
#include <cstdint>
#include "IOPort.h"

class StandardJoypad
{
public:
	enum class Button : uint8_t
	{
		A		= 1 << 7,
		B		= 1 << 6,
		Select	= 1 << 5,
		Start	= 1 << 4,
		Up		= 1 << 3,
		Down	= 1 << 2,
		Left	= 1 << 1,
		Right	= 1
	};

public:
	StandardJoypad(IOPort& inputPort)
		: _inputPort(inputPort)
	{
	}

	bool IsButttonDown(Button b) const
	{
		return _buttonsDown & static_cast<uint8_t>(b);
	}

	uint8_t ButtonsDown() const
	{
		return _buttonsDown;
	}

	uint8_t ShiftReg() const
	{
		return _shiftReg;
	}

	void SetButton(Button b, bool val);
	void SetStrobe(bool val);

	bool FetchButton();

private:
	uint8_t _shiftReg = 0;
	uint8_t _buttonsDown = 0;
	bool _strobe = false;
	IOPort& _inputPort;
};
#include "StandardJoypad.h"

void StandardJoypad::SetButton(StandardJoypad::Button b, bool val)
{
	const uint8_t mask = static_cast<uint8_t>(b);
	_buttonsDown = val ? _buttonsDown | mask : _buttonsDown & ~(mask);
}

void StandardJoypad::SetStrobe(bool val)
{
	if (!val && _strobe)
	{
		_shiftReg = _buttonsDown;
		_inputPort.SetPin(0, FetchButton(), false);
	}

	_strobe = val;
}

bool StandardJoypad::FetchButton()
{
	if (_strobe)
	{
		return _buttonsDown & static_cast<uint8_t>(Button::A);
	}

	const bool keyState = _shiftReg & (1 << 7);

	_shiftReg <<= 1;

	_inputPort.SetPin(0, _shiftReg & (1 << 7), false);

	return keyState;
}
#pragma once
#include <cstdint>

enum class PortDirection : uint8_t
{
	Input = 0x1f,
	Output = 0x07
};

class IOPort 
{
public:
	IOPort(PortDirection dir)
		: _mask(dir), _val(0)
	{
	}

	PortDirection Direction() const
	{
		return _mask;
	}

	uint8_t Get(bool silent = false)
	{
		if (!silent) _outputEnable = true;
		return _val;
	}

	void Set(uint8_t val, bool silent = false)
	{
		_val = val & static_cast<uint8_t>(_mask);
		if (!silent) _outputEnable = false;
	}

	bool GetPin(uint8_t n) const
	{
		return _val & (1 << n);
	}

	bool GetOE() const
	{
		return _outputEnable;
	}

	void SetPin(uint8_t n, bool set, bool silent = false)
	{
		if (set)
		{
			Set(_val | (1 << n), silent);
			return;
		}

		Set(_val & ~(1 << n), silent);
	}
private:
	PortDirection _mask;
	uint8_t _val;
	bool _outputEnable = false; // not emulating active low
};
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

	uint8_t Get() const
	{
		return _val;
	}

	void Set(uint8_t val)
	{
		_val = val & static_cast<uint8_t>(_mask);
	}

	bool GetPin(uint8_t n) const
	{
		return _val & (1 << n);
	}

	void SetPin(uint8_t n, bool val)
	{
		if (val)
		{
			Set(_val | (1 << n));
			return;
		}

		Set(_val & ~(1 << n));
	}
private:
	PortDirection _mask;
	uint8_t _val;
};
#pragma once
#include <cstdint>

class Bus
{
public:
	uint8_t Read(uint16_t addr);
	void Write(uint32_t addr, uint8_t data);
private:
};


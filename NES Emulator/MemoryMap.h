#pragma once
#include <cstdint>

class MemoryMap
{
public:
	virtual uint8_t Read(uint16_t addr) = 0;
	virtual void Write(uint32_t addr, uint8_t data) = 0;
};
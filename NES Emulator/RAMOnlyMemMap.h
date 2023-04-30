#pragma once
#include <cassert>
#include <memory>

#include "MemoryMap.h"

class RAMOnlyMemMap : public MemoryMap
{
public:

	RAMOnlyMemMap()
		: _ram(std::make_unique<uint8_t[]>(65536))
	{
	}


	uint8_t Read(uint16_t addr) override
	{
		assert(addr > 0 && addr < 65536);
		return _ram[addr];
	}

	void Write(uint32_t addr, uint8_t data) override
	{
		assert(addr >= 0 && addr < 65536);
		_ram[addr] = data;
	}

private:
	std::unique_ptr<uint8_t[]> _ram;
};

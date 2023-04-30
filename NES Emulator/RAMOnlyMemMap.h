#pragma once
#include <memory>
#include "MemoryMap.h"

class RAMOnlyMemMap : public MemoryMap
{
public:

	RAMOnlyMemMap()
		: _ram(std::make_unique<uint8_t[]>(65536))
	{
	}

	uint8_t Read(uint16_t addr) const override
	{
		return _ram[addr];
	}

	void Write(uint16_t addr, uint8_t data) override
	{
		_ram[addr] = data;
	}

	void LoadFromFile(const char* filePath);

public:
	static constexpr size_t RAM_SIZE = 65536;

private:
	std::unique_ptr<uint8_t[]> _ram;
};

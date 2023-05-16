#pragma once
#include <cstdint>
#include <memory>
#include "MemoryMap.h"

class Mapper0 : public MemoryMap
{
public:

	Mapper0(bool is32K, const std::unique_ptr<uint8_t[]>& programROM)
		: _ram(std::make_unique<uint8_t[]>(RAM_SIZE)), _programROM(programROM),
		_is32K(is32K)
	{
		
	}

	uint8_t Read(uint16_t addr) const override
	{
		if (addr <= 0x1fff)
		{
			return _ram[addr % RAM_SIZE];
		}

		if (addr >= 0x8000)
		{
			return _programROM[_is32K ? addr % 8000 : (addr % 8000) % PRG_ROM16K];
		}
		return 0;
	}

	void Write(uint16_t addr, uint8_t data) override
	{
		if (addr <= 0x1fff)
		{
			_ram[addr] = data;
		}
	}

public:
	static constexpr size_t RAM_SIZE = 2048;
	static constexpr size_t PRG_ROM16K = 16384;
	static constexpr size_t PRG_ROM32K = 32768;
private:
	std::unique_ptr<uint8_t[]> _ram;
	const std::unique_ptr<uint8_t[]>& _programROM;
	bool _is32K;
};

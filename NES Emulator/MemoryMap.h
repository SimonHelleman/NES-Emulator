#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "NametableMirroring.h"

class PPU;

class MemoryMap
{
public:
	virtual uint8_t Read(uint16_t addr, bool silent = false) const = 0;
	virtual void Write(uint16_t addr, uint8_t data) = 0;

	std::string HexDump();

	static constexpr size_t ADRESSABLE_RANGE = 65536;
};

class CPUMemoryMap : public MemoryMap
{
public:
	CPUMemoryMap(PPU* ppu)
		: _ram(std::make_unique<uint8_t[]>(RAM_SIZE)), _ppu(ppu)
	{
	}

public:
	static constexpr size_t RAM_SIZE = 2048;

protected:
	std::unique_ptr<uint8_t[]> _ram;
	PPU* _ppu;
};

class PPUMemoryMap : public MemoryMap
{
public:
	PPUMemoryMap(const std::unique_ptr<uint8_t[]>& chrROM, NametableMirroring mirroring)
		: _chrROM(chrROM), _nametableRAM(std::make_unique<uint8_t[]>(2048)),
		_palletRAM(std::make_unique<uint8_t[]>(32)), _mirroringMode(mirroring)
	{
		for (size_t i = 0; i < 32; ++i)
		{
			_palletRAM[i] = 0;
		}
	}

protected:
	const std::unique_ptr<uint8_t[]>& _chrROM;
	std::unique_ptr<uint8_t[]> _nametableRAM;
	std::unique_ptr<uint8_t[]> _palletRAM;

	NametableMirroring _mirroringMode;
};
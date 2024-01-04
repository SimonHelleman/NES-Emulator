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
	CPUMemoryMap(PPU* ppu, std::shared_ptr<uint8_t[]> programROM, size_t programROMSize)
		: _ram(std::make_unique<uint8_t[]>(RAM_SIZE)), _ppu(ppu), 
		_programROM(programROM), _programROMSize(programROMSize)
	{
	}

	void SetProgramROM(std::shared_ptr<uint8_t[]> programROM, size_t programROMSize)
	{
		_programROM = programROM;
		_programROMSize = programROMSize;
	}

public:
	static constexpr size_t RAM_SIZE = 2048;

protected:
	std::unique_ptr<uint8_t[]> _ram;
	PPU* _ppu;
	std::shared_ptr<uint8_t[]> _programROM;
	size_t _programROMSize;
};

class PPUMemoryMap : public MemoryMap
{
public:
	PPUMemoryMap(std::shared_ptr<uint8_t[]> chrROM, NametableMirroring mirroring)
		: _chrROM(chrROM), _nametableRAM(std::make_unique<uint8_t[]>(NAMETABLE_RAM_SIZE)),
		_palletRAM(), _mirroringMode(mirroring)
	{
		for (size_t i = 0; i < PALLET_RAM_SIZE; ++i)
		{
			_palletRAM[i] = 0;
		}
	}

	void SetCharacterROM(std::shared_ptr<uint8_t[]> chrROM)
	{
		_chrROM = chrROM;
	}

	void SetMirroringMode(NametableMirroring mirroring)
	{
		_mirroringMode = mirroring;
	}

public:
	static constexpr size_t PALLET_RAM_SIZE = 32;
	static constexpr size_t NAMETABLE_SIZE = 1024;
	static constexpr size_t NAMETABLE_RAM_SIZE = NAMETABLE_SIZE * 2;

protected:
	std::shared_ptr<uint8_t[]> _chrROM;
	std::unique_ptr<uint8_t[]> _nametableRAM;
	uint8_t _palletRAM[PALLET_RAM_SIZE];

	NametableMirroring _mirroringMode;
};
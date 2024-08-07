#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include "NametableMirroring.h"
#include "IOPort.h"

class PPU;

class MemoryMap
{
public:
	virtual uint8_t Read(uint16_t addr, bool silent = false) const = 0;
	virtual void Write(uint16_t addr, uint8_t data) = 0;

	virtual ~MemoryMap() = default;

	std::string HexDump();

	static constexpr size_t ADRESSABLE_RANGE = 65536;
};

class CPUMemoryMap : public MemoryMap
{
public:
	CPUMemoryMap(PPU* ppu, IOPort& outPort, IOPort& inPort0, IOPort& inPort1,
		std::shared_ptr<uint8_t[]> programROM, size_t programROMSize
	)
		: _ram(std::make_unique<uint8_t[]>(RAM_SIZE)),
		_programROM(std::move(programROM)), _programROMSize(programROMSize), _ppu(ppu),
		_outPort(outPort), _inPort0(inPort0), _inPort1(inPort1)
	{
	}

	void SetProgramROM(std::shared_ptr<uint8_t[]> programROM, size_t programROMSize)
	{
		_programROM = std::move(programROM);
		_programROMSize = programROMSize;
	}

public:
	static constexpr size_t RAM_SIZE = 2048;

protected:
	std::unique_ptr<uint8_t[]> _ram;
	std::shared_ptr<uint8_t[]> _programROM;
	size_t _programROMSize;
	PPU* _ppu;
	IOPort& _outPort;
	IOPort& _inPort0;
	IOPort& _inPort1;
};

class PPUMemoryMap : public MemoryMap
{
public:
	PPUMemoryMap(std::shared_ptr<uint8_t[]> chrROM, NametableMirroring mirroring)
		: _chrROM(std::move(chrROM)), _nametableRAM(std::make_unique<uint8_t[]>(NAMETABLE_RAM_SIZE)),
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
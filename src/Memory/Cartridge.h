#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "NametableMirroring.h"

class Cartridge
{
public:
	Cartridge();
	Cartridge(const char* filePath);

	size_t ProgramROMSize() const
	{
		return _programROMSize;
	}

	size_t ChracterROMSize() const
	{
		return _characterROMSize;
	}
	
	const std::shared_ptr<uint8_t[]> ProgramROM() const
	{
		return _programROM;
	}

	const std::shared_ptr<uint8_t[]> CharacterROM() const
	{
		return _characterROM;
	}

	int Mapper() const
	{
		return _mapper;
	}

	NametableMirroring MirroringMode() const
	{
		return _mirroringMode;
	}

	std::string Name()
	{
		return _name;
	}

private:
	struct Header
	{
		char name[4];
		uint8_t programROMUnits;
		uint8_t characterROMUnits;
		uint8_t flags06; // mapper, mirroring, battery, trainer
		uint8_t flags07; // mapper, vs/playchoice, nes 2.0
		uint8_t programRAMSize;
		uint8_t tvSystem; // TV system
		uint8_t flags10; // TV system, prg-ram presence
		uint8_t padding[5];
	};

	static constexpr size_t PROGRAM_ROM_UNIT = 16384;
	static constexpr size_t CHARACTER_ROM_UNIT = 8192;

private:
	std::string _name = "default";
	int _mapper = 0;
	NametableMirroring _mirroringMode = NametableMirroring::Vertical;
	size_t _programROMSize = PROGRAM_ROM_UNIT;
	size_t _characterROMSize = CHARACTER_ROM_UNIT;
	std::shared_ptr<uint8_t[]> _programROM = std::make_unique<uint8_t[]>(PROGRAM_ROM_UNIT);
	std::shared_ptr<uint8_t[]> _characterROM = std::make_unique<uint8_t[]>(CHARACTER_ROM_UNIT);
};


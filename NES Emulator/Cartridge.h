#pragma once
#include <cstdint>
#include <memory>
class Cartridge
{
public:
	Cartridge(const char* filePath);
	
	const std::unique_ptr<uint8_t[]>& ProgramROM() const
	{
		return _programROM;
	}

	const std::unique_ptr<uint8_t[]>& CharacterROM() const
	{
		return _characterROM;
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
	int _mapper;
	std::unique_ptr<uint8_t[]> _programROM;
	std::unique_ptr<uint8_t[]> _characterROM;
};


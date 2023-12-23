#include <iostream>
#include <fstream>
#include "Cartridge.h"

Cartridge::Cartridge(const char* filePath)
{
	Header header;

	std::ifstream file(filePath, std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error opening file: " << filePath << '\n';
		return;
	}

	file.read(reinterpret_cast<char*>(&header), sizeof(Header));

	if (header.flags06 & 0b00000100) // skip trainer information
	{
		file.seekg(512, std::ios::cur);
	}

	_mapper = ((header.flags07 >> 4) << 4) | (header.flags06 >> 4);
	_mirroringMode = header.flags06 & 0b00000001 ? NametableMirroring::Vertical : NametableMirroring::Horizontal;

	_programROMSize = header.programROMUnits * PROGRAM_ROM_UNIT;
	_characterROMSize = header.characterROMUnits * CHARACTER_ROM_UNIT;

	_programROM = std::make_unique<uint8_t[]>(_programROMSize);
	_characterROM = std::make_unique<uint8_t[]>(_characterROMSize);

	file.read(reinterpret_cast<char*>(_programROM.get()), _programROMSize);
	file.read(reinterpret_cast<char*>(_characterROM.get()), _characterROMSize);

	file.close();
}

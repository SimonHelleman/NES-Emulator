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

	size_t programROMSize = header.programROMUnits * PROGRAM_ROM_UNIT;
	size_t characterROMSize = header.characterROMUnits * CHARACTER_ROM_UNIT;

	_programROM = std::make_unique<uint8_t[]>(programROMSize);
	_characterROM = std::make_unique<uint8_t[]>(characterROMSize);

	file.read(reinterpret_cast<char*>(_programROM.get()), programROMSize);
	file.read(reinterpret_cast<char*>(_characterROM.get()), characterROMSize);

	file.close();

}

#include <cstring>
#include <fstream>
#include "Logger.h"
#include "Cartridge.h"

Cartridge::Cartridge()
{
	memset(_programROM.get(), 0xea, _programROMSize);
}

Cartridge::Cartridge(const char* filePath)
{
	_name = std::string(filePath);
	_name = _name.substr(_name.find_last_of("\\/") + 1);

	Header header;

	std::ifstream file(filePath, std::ios::binary);

	if (!file.is_open())
	{
		ERROR(std::string("[CART]Error opening file: ") + _name);
		memset(_programROM.get(), 0xea, _programROMSize);
		return;
	}

	file.read(reinterpret_cast<char*>(&header), sizeof(Header));

	if (header.name[0] != 'N' || header.name[1] != 'E' || header.name[2] != 'S')
	{
		ERROR(std::string("[CART]") + _name + " is not a valid iNES file");
		memset(_programROM.get(), 0xea, _programROMSize);
		return;
	}

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

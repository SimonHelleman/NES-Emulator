#include <sstream>
#include "MemoryMap.h"

std::string MemoryMap::HexDump()
{
	char buf[64];

	std::stringstream ss;

	for (int i = 0; i < ADRESSABLE_RANGE; ++i)
	{
		if (i % 16 == 0)
		{
			if (i > 0)
			{
				ss << '\n';
			}
			snprintf(buf, 64, "%04x: ", i);
			ss << buf;
		}

		snprintf(buf, 64, "%02x ", Read(i));
		ss << buf;
	}

	return ss.str();
}

void CPUMemoryMap::Reset()
{
	_ram = std::make_unique<uint8_t[]>(RAM_SIZE);
}

void PPUMemoryMap::Reset()
{
	_nametableRAM = std::make_unique<uint8_t[]>(NAMETABLE_RAM_SIZE);
	
	for (size_t i = 0; i < PALLET_RAM_SIZE; ++i)
	{
		_palletRAM[i] = 0;
	}
}

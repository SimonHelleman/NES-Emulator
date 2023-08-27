#include "Mapper0.h"
#include <iostream>

uint8_t CPUMapper0::Read(uint16_t addr) const
{
	if (addr <= 0x1fff) return _ram[addr % RAM_SIZE];
	if (addr == 0x2002) return _ppu->ReadStatus();
	if (addr == 0x2004) return _ppu->ReadOAMData();
	if (addr == 0x2007) return _ppu->ReadData();
	if (addr >= 0x8000)
	{
		bool is32K = _programROMSize == PRG_ROM32K;
		uint16_t index = is32K ? addr % 0x8000 : (addr % 0x8000) % PRG_ROM16K;
		//uint16_t index = addr & (_is32K ? PRG_ROM32K - 1 : PRG_ROM16K - 1);
		return _programROM[index];
	}

	return 0;
}

void CPUMapper0::Write(uint16_t addr, uint8_t data)
{
	if (addr <= 0x1fff) _ram[addr] = data;
	if (addr == 0x2000) _ppu->WriteControl(data);
	if (addr == 0x2001) _ppu->WriteMask(data);
	if (addr == 0x2003) _ppu->WriteOAMAddr(data);
	if (addr == 0x2004) _ppu->WriteOAMData(data);
	if (addr == 0x2005) _ppu->WriteScroll(data);
	if (addr == 0x2006) _ppu->WriteAddress(data);
	if (addr == 0x2007) _ppu->WriteData(data);
}

uint8_t PPUMapper0::Read(uint16_t addr) const
{
	if (addr <= 0x1fff)
	{
		return _chrROM[addr];
	}

	if (addr >= 0x3f00 && addr <= 0x3fff)
	{
		return _palletRAM[addr % 0x3f00];
	}
	
	return 0;
}

void PPUMapper0::Write(uint16_t addr, uint8_t data)
{
	if (addr <= 0x1fff)
	{
		_chrROM[addr] = data;
	}

	if (addr >= 0x3f00 && addr <= 0x3fff)
	{
		// TODO: Buffer overrun bug here
		std::cout << "pallet write: " << addr % 0x3f00 << '\n';
		_palletRAM[addr % 0x3f00] = data;
	}

}

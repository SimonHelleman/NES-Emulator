#include "Mapper0.h"

uint8_t CPUMapper0::Read(uint16_t addr) const
{
	if (addr <= 0x1fff) return _ram[addr % RAM_SIZE];
	if (addr == 0x2002) return _ppu->ReadStatus();
	if (addr == 0x2004) return _ppu->ReadOAMData();
	if (addr >= 0x8000)
	{
		uint16_t index = _is32K ? addr % 0x8000 : (addr % 0x8000) % PRG_ROM16K;
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
}

void PPUMapper0::Write(uint16_t addr, uint8_t data)
{
	if (addr <= 0x1fff)
	{
		_chrROM[addr] = data;
	}

}

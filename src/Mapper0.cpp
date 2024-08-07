#include "Logger.h"
#include "Mapper0.h"

uint8_t CPUMapper0::Read(uint16_t addr, bool silent) const
{
	if (addr <= 0x1fff) return _ram[addr % RAM_SIZE];
	if (addr >= 0x2000 && addr <= 0x3fff)
	{
		addr = 0x2000 + (addr % 8);
		if (addr == 0x2002) return _ppu->ReadStatus(silent);
		if (addr == 0x2004) return _ppu->ReadOAMData();
		if (addr == 0x2007) return _ppu->ReadData();
	}
	
	if (addr == 0x4016) return _inPort0.Get();
	if (addr == 0x4017) return _inPort1.Get();

	// Not yet implemented... supress unmapped warning for now
	if (addr >= 0x4000 && addr <= 0x4017)
	{
		return 0;
	}

	if (addr >= 0x8000)
	{
		const bool is32K = _programROMSize == PRG_ROM32K;
		const uint16_t index = is32K ? addr % 0x8000 : (addr % 0x8000) % PRG_ROM16K;
		//uint16_t index = addr & (_is32K ? PRG_ROM32K - 1 : PRG_ROM16K - 1);
		return _programROM[index];
	}

	if (!silent)
	{
		WARN("[MAP0_CPU] Unmapped read from 0x" + Int16HexString(addr));
	}
	return 0;
}

void CPUMapper0::Write(uint16_t addr, uint8_t data)
{
	if (addr <= 0x1fff)
	{
		_ram[addr % RAM_SIZE] = data;
		return;
	}

	if (addr >= 0x2000 && addr <= 0x3fff)
	{
		addr = 0x2000 + (addr % 8);
		if (addr == 0x2000) _ppu->WriteControl(data);
		if (addr == 0x2001) _ppu->WriteMask(data);
		if (addr == 0x2003) _ppu->WriteOAMAddr(data);
		if (addr == 0x2004) _ppu->WriteOAMData(data);
		if (addr == 0x2005) _ppu->WriteScroll(data);
		if (addr == 0x2006) _ppu->WriteAddress(data);
		if (addr == 0x2007) _ppu->WriteData(data);

		return;
	}

	if (addr == 0x4016) _outPort.Set(data);

	// APU stuff.. Not today...
	if (addr >= 0x4000 && addr <= 0x4017)
	{
		return;
	}

	WARN("[MAP0_CPU] Unmapped write to 0x" + Int16HexString(addr));
}

uint8_t PPUMapper0::Read(uint16_t addr, bool silent) const
{
	if (addr <= 0x1fff)
	{
		return _chrROM[addr];
	}

	if (addr < 0x2800 && _mirroringMode == NametableMirroring::Horizontal)
	{
		return _nametableRAM[addr % NAMETABLE_SIZE];
	}

	if (addr >= 0x2800 && addr <= 0x2fff && _mirroringMode == NametableMirroring::Horizontal)
	{
		const uint16_t index = (addr % NAMETABLE_SIZE) + NAMETABLE_SIZE;
		return _nametableRAM[index];
	}

	if (addr >= 0x2000 && addr <= 0x2fff && _mirroringMode == NametableMirroring::Vertical)
	{
		return _nametableRAM[addr % NAMETABLE_RAM_SIZE];
	}

	if (addr >= 0x3f00 && addr <= 0x3fff)
	{
		return _palletRAM[addr % PALLET_RAM_SIZE];
	}
	
	if (!silent)
	{
		WARN("[MAP0_PPU] Unmapped read from 0x" + Int16HexString(addr));
	}
	return 0;
}

void PPUMapper0::Write(uint16_t addr, uint8_t data)
{
	if (addr <= 0x1fff)
	{
		_chrROM[addr] = data;
		return;
	}

	if (addr < 0x2800 && _mirroringMode == NametableMirroring::Horizontal)
	{
		_nametableRAM[addr % NAMETABLE_SIZE] = data;
		return;
	}

	if (addr >= 0x2800 && addr <= 0x2fff && _mirroringMode == NametableMirroring::Horizontal)
	{
		const uint16_t index = (addr % NAMETABLE_SIZE) + NAMETABLE_SIZE;
		_nametableRAM[index] = data;
		return;
	}

	if (addr >= 0x2000 && addr <= 0x2fff && _mirroringMode == NametableMirroring::Vertical)
	{
		_nametableRAM[addr % NAMETABLE_RAM_SIZE] = data;
		return;
	}

	if (addr >= 0x3f00 && addr <= 0x3fff)
	{
		//std::cout << "pallet write: index = " << addr % 32 << " data = " << (int)data << '\n';
		if (data >= PPU::PALETTE_LEN)
		{
			WARN("[MAP0_PPU] Palette index written falls outside of bounds! Write ignored.")
			return;
		}
		_palletRAM[addr % PALLET_RAM_SIZE] = data;
		return;
	}

	WARN("[MAP0_PPU] Unmapped write to 0x" + Int16HexString(addr));
}

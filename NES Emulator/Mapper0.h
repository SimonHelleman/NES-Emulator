#pragma once
#include "PPU.h"
#include "MemoryMap.h"

class CPUMapper0 : public MemoryMap
{
public:

	CPUMapper0(PPU* ppu, bool is32K, const std::unique_ptr<uint8_t[]>& programROM)
		: _ram(std::make_unique<uint8_t[]>(RAM_SIZE)), _programROM(programROM),
		_ppu(ppu), _is32K(is32K)
	{
	}

	uint8_t Read(uint16_t addr) const override;

	void Write(uint16_t addr, uint8_t data) override;
	
public:
	static constexpr size_t RAM_SIZE = 2048;
	static constexpr size_t PRG_ROM16K = 16384;
	static constexpr size_t PRG_ROM32K = 32768;
private:
	std::unique_ptr<uint8_t[]> _ram;
	const std::unique_ptr<uint8_t[]>& _programROM;
	PPU* _ppu;
	bool _is32K;
};

class PPUMapper0 : public PPUMemoryMap
{
public:
	
	PPUMapper0(const std::unique_ptr<uint8_t[]>& chrROM)
		: PPUMemoryMap(chrROM)
	{
	}

	uint8_t Read(uint16_t addr) const override;
	void Write(uint16_t addr, uint8_t data) override;

};

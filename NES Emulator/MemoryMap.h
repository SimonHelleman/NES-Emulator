#pragma once
#include <cstdint>
#include <string>

class MemoryMap
{
public:
	virtual uint8_t Read(uint16_t addr) const = 0;
	virtual void Write(uint16_t addr, uint8_t data) = 0;

	std::string HexDump();

	static constexpr size_t ADRESSABLE_RANGE = 65536;
};

class PPUMemoryMap : public MemoryMap
{
public:
	PPUMemoryMap(const std::unique_ptr<uint8_t[]>& chrROM)
		: _chrROM(chrROM), _nametableRAM(std::make_unique<uint8_t[]>(2048)),
		_palletRAM(std::make_unique<uint8_t[]>(32))
	{
	}

	virtual uint8_t Read(uint16_t addr) const = 0;
	virtual void Write(uint16_t addr, uint8_t data) = 0;

protected:
	const std::unique_ptr<uint8_t[]>& _chrROM;
	std::unique_ptr<uint8_t[]> _nametableRAM;
	std::unique_ptr<uint8_t[]> _palletRAM;
};
#include <cassert>
#include <iostream>
#include "PPU.h"

void PPU::Clock()
{
    if (_scanline < 240 && _cycles < 256 && _cycles > 0)
    {

    }

    if (++_cycles >= CYCLES_PER_SCANLINE)
    {
        _cycles = 0;
        if (++_scanline >= SCANLINES_PER_FRAME)
        {
            _scanline = -1;
        }
    }
}

void PPU::WriteControl(uint8_t val)
{
	_regControl = val;
}

void PPU::WriteMask(uint8_t val)
{
	_regMask = val;
}

uint8_t PPU::ReadStatus()
{
	uint8_t val = _regStatus;
	_regStatus &= ~STATUS_V;
	_addressLatch = false;

    _regStatus |= STATUS_V;

	return val;
}

void PPU::WriteOAMAddr(uint8_t val)
{
	_regOAMAddr = val;
}

uint8_t PPU::ReadOAMData()
{
	return _regOAMData;
}

void PPU::WriteOAMData(uint8_t val)
{
	_regOAMData = val;
}

void PPU::WriteScroll(uint8_t val)
{
	uint16_t val16 = val;
	_regAddr |= _addressLatch ? val : val16 << 8;
	_addressLatch = !_addressLatch;
}

void PPU::WriteAddress(uint8_t val)
{
	uint16_t val16 = val;
	_regAddr |= _addressLatch ? val : val16 << 8;
	_addressLatch = !_addressLatch;
}

void PPU::WriteData(uint8_t val)
{
	_memory.Write(_regAddr, val);
	_regAddr += (_regControl & CONTROL_I) ? 32 : 1;
}

uint8_t PPU::ReadData()
{
	// Reads before pallet RAM are delayed by 1 cycle
	uint8_t ret = 0;
	if (_regAddr < 0x3f00)
	{
		ret = _regData;
		_regData = _memory.Read(_regAddr); // Delayed read
		_regAddr += (_regControl & CONTROL_I) ? 32 : 1;
		return ret;
	}

	// Palette memory is an exception
	ret = _memory.Read(_regAddr);
	_regAddr += (_regControl & CONTROL_I) ? 32 : 1;
	return ret;
}

PPU::Palette PPU::GetPalette(int paletteIndex)
{
    uint16_t startAddr = 0x3f01 + (4 * paletteIndex);

    return PPU::Palette(PALETTE[_memory.Read(startAddr)], PALETTE[_memory.Read(startAddr + 1)], PALETTE[_memory.Read(startAddr + 2)]);
}


Image PPU::GetTile(uint8_t index, const Palette& palette)
{
    Image tile = Image(8, 8);
    uint16_t startAddr = 16 * index;

    uint8_t tileData[2][8];

    size_t offset = 0;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            tileData[i][j] = _memory.Read(startAddr + offset++);
        }
    }

    for (int y = 0; y < tile.Height(); ++y)
    {
        for (int x = 0; x < tile.Width(); ++x)
        {
            uint8_t msb = (tileData[1][y] >> (7 - x)) & 0b00000001;
            uint8_t lsb = (tileData[0][y] >> (7 - x)) & 0b00000001;
            uint8_t palletIndex = msb << 1 | lsb;

            tile.SetPixel(x, y, palette.color[palletIndex]);
        }
    }

    return tile;
}


Image PPU::GetPatternTable(int table, const Palette& palette)
{
    assert(table < 2);

    constexpr unsigned int DIMENSION = 16 * 8;
    Image image = Image(DIMENSION, DIMENSION);

    uint16_t tableStart = table * 4096;

    for (size_t tile = 0; tile < 256; ++tile)
    {
        uint16_t tileStart = tableStart + 16 * tile;
        uint8_t tileData[2][8];

        uint16_t offset = 0;

        for (size_t i = 0; i < 2; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                tileData[i][j] = _memory.Read(tileStart + offset++);
            }
        }

        for (int y = 0; y < 8; ++y)
        {
            for (int x = 0; x < 8; ++x)
            {
                uint8_t msb = (tileData[1][y] >> (7 - x)) & 0b00000001;
                uint8_t lsb = (tileData[0][y] >> (7 - x)) & 0b00000001;
                uint8_t palletIndex = msb << 1 | lsb;
                image.SetPixel(8 * (tile % 16) + x, 8 * ((tile / 16) % 16) + y, palette.color[palletIndex]);
            }
        }

    }

    return image;
}

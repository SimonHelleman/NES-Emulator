#include <cassert>
#include <iostream>
#include "PPU.h"

void PPU::Clock()
{
}

Image PPU::GetTile(uint8_t index, const Pallet& pallet)
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

            tile.SetPixel(x, y, pallet.color[palletIndex]);
        }
    }

    return tile;
}

Image PPU::GetPatternTable(int table, const Pallet& pallet)
{
    assert(table < 2);

    constexpr unsigned int DIMENSION = 16 * 8;
    Image image = Image(DIMENSION, DIMENSION);
    image.Clear(0xffffffff);

    uint16_t tableStart = table * 4096;

    for (size_t tile = 0; tile < 256; ++tile)
    {
        uint16_t tileStart = tableStart + 16 * tile;
        uint8_t tileData[2][8];

        size_t offset = 0;

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 8; ++j)
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
                image.SetPixel(8 * (tile % 16) + x, 8 * ((tile / 16) % 16) + y, pallet.color[palletIndex]);
            }
        }

    }

    return image;
}

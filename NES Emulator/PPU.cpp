#include "PPU.h"

void PPU::Clock()
{
}

Image PPU::GetBackgroundTile(uint8_t index, const Pallet& pallet)
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

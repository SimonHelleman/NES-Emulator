#pragma once
#include <cstdint>
#include <memory>
#include "MemoryMap.h"
#include "Image.h"

class PPU
{
//private:
public:
	struct Pallet
	{
		RGBA color[4];

		Pallet(RGBA color1, RGBA color2, RGBA color3)
		{
			color[0] = 0xff000000;
			color[1] = color1;
			color[2] = color2;
			color[3] = color3;
		}
	};

public:
	PPU(MemoryMap& memory, const std::unique_ptr<uint8_t[]>& chrROM)
		: _memory(memory), _framebuffer(256, 240), _chrROM(chrROM)
	{

	}

	const Image& GetFramebuffer()
	{
		return _framebuffer;
	}

	uint8_t Read(uint16_t addr);
	void Write(uint16_t addr, uint8_t data);

	Image GetBackgroundTile(uint8_t index, const Pallet& pallet);

private:
	MemoryMap& _memory;
	Image _framebuffer;

	const std::unique_ptr<uint8_t[]>& _chrROM;
	std::unique_ptr<uint8_t[]> _nametableRAM;
	std::unique_ptr<uint8_t[]> _palletRAM;
	//std::unique_ptr<uint8_t[]> _objectAttributeMemory;

	static constexpr int PALLET_LEN = 0x40;
	static constexpr RGBA PALLET[] = {
		{  84,  84,  84 }, {   0,  30, 116 }, {   8,  16, 144 }, {  48,   0, 136 }, {  68,   0, 100 }, {  92,   0,  48 }, {  84,   4,   0 }, {  60,  24,   0 }, {  32,  42,   0 }, {   8,  58,   0 }, {   0,  64,   0 }, {   0,  60,   0 }, {   0,  50,  60 }, {   0,   0,   0 },
		{ 152, 150, 152 }, {   8,  76, 196 }, {  48,  50, 236 }, {  92,  30, 228 }, { 136,  20, 176 }, { 160,  20, 100 }, { 152,  34,  32 }, { 120,  60,   0 }, {  84,  90,   0 }, {  40, 114,   0 }, {   8, 124,   0 }, {   0, 118,  40 }, {   0, 102, 120 }, {   0,   0,   0 },
		{ 236, 238, 236 }, {  76, 154, 236 }, { 120, 124, 236 }, { 176,  98, 236 }, { 228,  84, 236 }, { 236,  88, 180 }, { 236, 106, 100 }, { 212, 136,  32 }, { 160, 170,   0 }, { 116, 196,   0 }, {  76, 208,  32 }, {  56, 204, 108 }, {  56, 180, 204 }, {  60,  60,  60 },
		{ 236, 238, 236 }, { 168, 204, 236 }, { 188, 188, 236 }, { 212, 178, 236 }, { 236, 174, 236 }, { 236, 174, 212 }, { 236, 180, 176 }, { 228, 196, 144 }, { 204, 210, 120 }, { 180, 222, 120 }, { 168, 226, 144 }, { 152, 226, 180 }, { 160, 214, 228 }, { 160, 162, 160 }
	};
};


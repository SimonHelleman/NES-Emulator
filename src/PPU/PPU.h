#pragma once
#include <cstdint>
#include <memory>
#include "Memory/MemoryMap.h"
#include "Util/Image.h"

class PPU
{
//private:
public:
	struct Palette
	{
		RGBA color[4];

		Palette(RGBA color1, RGBA color2, RGBA color3, RGBA bg = 0x00000000)
		{
			color[0] = bg;
			color[1] = color1;
			color[2] = color2;
			color[3] = color3;
		}
	};

public:
	PPU(PPUMemoryMap& memory)
		: _memory(memory), _framebuffer(VISIBLE_PIXELS, VISIBLE_SCANLINES)
	{
	}

	void Clock();

	void Reset()
	{
		_addressLatch = false;
	}

	const Image& GetFramebuffer()
	{
		return _framebuffer;
	}

	bool IsFrameComplete()
	{
		return _frameComplete;
	}

	bool PendingNMI()
	{
		return _doNMI;
	}

	void ClearNMI()
	{
		_doNMI = false;
	}

	void WriteControl(uint8_t val);
	void WriteMask(uint8_t val);
	uint8_t ReadStatus(bool silent = false);
	void WriteOAMAddr(uint8_t val);
	uint8_t ReadOAMData();
	void WriteOAMData(uint8_t val);
	void WriteScroll(uint8_t val);
	void WriteAddress(uint8_t val);
	void WriteData(uint8_t val);
	uint8_t ReadData();
	
	Palette GetPalette(int paletteIndex) const;

	Image GetTile(int table, uint8_t index, const Palette& palette) const;
	Image GetPatternTable(int table, const Palette& palette) const;

	uint16_t AddressReg() const
	{
		return _regAddr;
	}

	uint8_t DataReg() const
	{
		return _regData;
	}

	uint8_t ControlReg() const
	{
		return _regControl;
	}

private:
	PPUMemoryMap& _memory;

	uint8_t _regControl = 0;
	uint8_t _regMask = 0;
	uint8_t _regStatus = 0;
	uint8_t _regOAMAddr = 0;
	uint8_t _regOAMData = 0;
	uint16_t _regScroll = 0;
	uint16_t _regAddr = 0;
	uint8_t _regData = 0;
	uint8_t _regOAMDMA = 0;

	bool _addressLatch = false;

	int _cycles = 0;
	int _scanline = 0;
	bool _doNMI = false;
	bool _frameComplete = false;

	
	Image _framebuffer;

public:
	static constexpr int PALETTE_LEN = 0x40;
	static constexpr RGBA PALETTE[] = {
		{  84,  84,  84 }, {   0,  30, 116 }, {   8,  16, 144 }, {  48,   0, 136 }, {  68,   0, 100 }, {  92,   0,  48 }, {  84,   4,   0 }, {  60,  24,   0 }, {  32,  42,   0 }, {   8,  58,   0 }, {   0,  64,   0 }, {   0,  60,   0 }, {   0,  50,  60 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
		{ 152, 150, 152 }, {   8,  76, 196 }, {  48,  50, 236 }, {  92,  30, 228 }, { 136,  20, 176 }, { 160,  20, 100 }, { 152,  34,  32 }, { 120,  60,   0 }, {  84,  90,   0 }, {  40, 114,   0 }, {   8, 124,   0 }, {   0, 118,  40 }, {   0, 102, 120 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 },
		{ 236, 238, 236 }, {  76, 154, 236 }, { 120, 124, 236 }, { 176,  98, 236 }, { 228,  84, 236 }, { 236,  88, 180 }, { 236, 106, 100 }, { 212, 136,  32 }, { 160, 170,   0 }, { 116, 196,   0 }, {  76, 208,  32 }, {  56, 204, 108 }, {  56, 180, 204 }, {  60,  60,  60 }, {   0,   0,   0 }, {   0,   0,   0 },
		{ 236, 238, 236 }, { 168, 204, 236 }, { 188, 188, 236 }, { 212, 178, 236 }, { 236, 174, 236 }, { 236, 174, 212 }, { 236, 180, 176 }, { 228, 196, 144 }, { 204, 210, 120 }, { 180, 222, 120 }, { 168, 226, 144 }, { 152, 226, 180 }, { 160, 214, 228 }, { 160, 162, 160 }, {   0,   0,   0 }, {   0,   0,   0 }
	};
	
	static constexpr uint8_t CONTROL_V = 0b10000000;
	static constexpr uint8_t CONTROL_P = 0b01000000;
	static constexpr uint8_t CONTROL_H = 0b00100000;
	static constexpr uint8_t CONTROL_B = 0b00010000;
	static constexpr uint8_t CONTROL_S = 0b00001000;
	static constexpr uint8_t CONTROL_I = 0b00000100;
	static constexpr uint8_t CONTROL_2 = 0b00000010;
	static constexpr uint8_t CONTROL_1 = 0b00000001;

	static constexpr uint8_t MASK_EMPH_BLUE            = 0b10000000;
	static constexpr uint8_t MASK_EMPH_GREEN           = 0b01000000;
	static constexpr uint8_t MASK_EMPH_RED             = 0b00100000;
	static constexpr uint8_t MASK_SHOW_SPRITES         = 0b00010000;
	static constexpr uint8_t MASK_SHOW_BACKGROUND      = 0b00001000;
	static constexpr uint8_t MASK_SHOW_SPRITES_LEFT    = 0b00000100;
	static constexpr uint8_t MASK_SHOW_BACKGROUND_LEFT = 0b00000010;
	static constexpr uint8_t MASK_SHOW_GRAYSCALE       = 0b00000001;

	static constexpr uint8_t STATUS_V = 0b10000000;
	static constexpr uint8_t STATUS_S = 0b01000000;
	static constexpr uint8_t STATUS_O = 0b00100000;

	static constexpr int VISIBLE_SCANLINES = 240;
	static constexpr int VISIBLE_PIXELS = 256;
	static constexpr int CYCLES_PER_SCANLINE = 341;
	static constexpr int SCANLINES_PER_FRAME = 261;
};


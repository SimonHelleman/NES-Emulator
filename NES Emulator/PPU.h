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
			color[0] = 0x00000000;
			color[1] = color1;
			color[2] = color2;
			color[3] = color3;
		}
	};

public:
	PPU(PPUMemoryMap& memory)
		: _memory(memory), _framebuffer(256, 240)
	{

	}

	void Clock();

	const Image& GetFramebuffer()
	{
		return _framebuffer;
	}

	void WriteControl(uint8_t val)
	{
		_regControl = val;
	}

	void WriteMask(uint8_t val)
	{
		_regMask = val;
	}

	uint8_t ReadStatus()
	{
		uint8_t val = _regStatus;
		_regStatus &= ~STATUS_V;
		_addressLatch = false;

		return val;
	}

	void WriteOAMAddr(uint8_t val)
	{
		_regOAMAddr = val;
	}

	uint8_t ReadOAMData()
	{
		return _regOAMData;
	}

	void WriteOAMData(uint8_t val)
	{
		_regOAMData = val;
	}

	void WriteScroll(uint8_t val)
	{
		uint16_t val16 = val;
		_regAddr |= _addressLatch ? val : val16 << 8;
		_addressLatch = !_addressLatch;
	}

	void WriteAddress(uint8_t val)
	{
		uint16_t val16 = val;
		_regAddr |= _addressLatch ? val : val16 << 8;
		_addressLatch = !_addressLatch;
	}

	void WriteData(uint8_t val)
	{
		_memory.Write(_regAddr, val);
		_regAddr += (_regControl | 0b00000100) ? 32 : 1;
	}

	uint8_t ReadData()
	{
		// Reads before pallet RAM are delayed by 1 cycle
		uint8_t ret = 0;
		if (_regAddr < 0x3f00)
		{
			ret = _regData;
			_regData = _memory.Read(_regAddr); // Delayed read
			_regAddr += (_regControl | 0b00000100) ? 32 : 1;
			return ret;
		}

		// Pallet memory is an exception
		ret = _memory.Read(_regAddr);
		_regAddr += (_regControl | 0b00000100) ? 32 : 1;
		return ret;
	}

	Pallet GetPallet(int palletIndex);

	Image GetTile(uint8_t index, const Pallet& pallet);
	Image GetPatternTable(int table, const Pallet& pallet);

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
	
	Image _framebuffer;

private:
	static constexpr int PALLET_LEN = 0x40;
	static constexpr RGBA PALLET[] = {
		{  84,  84,  84 }, {   0,  30, 116 }, {   8,  16, 144 }, {  48,   0, 136 }, {  68,   0, 100 }, {  92,   0,  48 }, {  84,   4,   0 }, {  60,  24,   0 }, {  32,  42,   0 }, {   8,  58,   0 }, {   0,  64,   0 }, {   0,  60,   0 }, {   0,  50,  60 }, {   0,   0,   0 },
		{ 152, 150, 152 }, {   8,  76, 196 }, {  48,  50, 236 }, {  92,  30, 228 }, { 136,  20, 176 }, { 160,  20, 100 }, { 152,  34,  32 }, { 120,  60,   0 }, {  84,  90,   0 }, {  40, 114,   0 }, {   8, 124,   0 }, {   0, 118,  40 }, {   0, 102, 120 }, {   0,   0,   0 },
		{ 236, 238, 236 }, {  76, 154, 236 }, { 120, 124, 236 }, { 176,  98, 236 }, { 228,  84, 236 }, { 236,  88, 180 }, { 236, 106, 100 }, { 212, 136,  32 }, { 160, 170,   0 }, { 116, 196,   0 }, {  76, 208,  32 }, {  56, 204, 108 }, {  56, 180, 204 }, {  60,  60,  60 },
		{ 236, 238, 236 }, { 168, 204, 236 }, { 188, 188, 236 }, { 212, 178, 236 }, { 236, 174, 236 }, { 236, 174, 212 }, { 236, 180, 176 }, { 228, 196, 144 }, { 204, 210, 120 }, { 180, 222, 120 }, { 168, 226, 144 }, { 152, 226, 180 }, { 160, 214, 228 }, { 160, 162, 160 }
	};
	

public:
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
};


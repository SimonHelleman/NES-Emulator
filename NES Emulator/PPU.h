#pragma once
#include <cstdint>
#include "MemoryMap.h"
#include "Image.h"

class PPU
{
	PPU(MemoryMap& memory)
		: _memory(memory), _framebuffer(256, 240)
	{

	}

	const Image& GetFramebuffer()
	{
		return _framebuffer;
	}
private:
	MemoryMap& _memory;
	Image _framebuffer;
};


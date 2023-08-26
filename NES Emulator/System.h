#pragma once
#include <cstdint>
#include "Cartridge.h"
#include "Mapper0.h"
#include "CPU.h"
#include "PPU.h"

class System
{
public:

	System(const Cartridge& cart);
	~System();

	System(const System&) = delete;
	System& operator=(const System&) = delete;

	System(System&&) = delete;
	System& operator=(System&&) = delete;


	void Run(bool run)
	{
		_continuousRun = run;
	}


	void ClockStep();
	void InstructionStep();

private:
	void SystemClock();
private:

	const Cartridge& _cart;

	MemoryMap* _memoryCPU;
	Disassembler* _disassembler;
	CPU* _cpu;

	PPUMemoryMap* _memoryPPU;
	PPU* _ppu;

	bool _continuousRun = false;
	uint64_t _cycleCount = 0;
};


#pragma once
#include <cstdint>
#include <set>
#include "Cartridge.h"
#include "Mapper0.h"
#include "Disassembler.h"
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

	bool IsRunning()
	{
		return _continuousRun;
	}

	void EnableBreakpoints(bool enable)
	{
		_enableBreakpoints = enable;
	}

	uint64_t CycleCount() const
	{
		return _cycleCount;
	}

	void Reset();

	void Update();

	void ClockStep();
	void InstructionStep();
	void FrameStep();

	void AddBreakpoint(uint16_t addr);
	void RemoveBreakpoint(uint16_t addr);
	const std::set<uint16_t>& GetBreakpoints() const;

public:
	MemoryMap* CPUMemory() const
	{
		return _memoryCPU;
	}

	Disassembler* GetDissembler() const
	{
		return _disassembler;
	}

	CPU* GetCPU() const
	{
		return _cpu;
	}

	PPUMemoryMap* PPUMemory() const
	{
		return _memoryPPU;
	}

	PPU* GetPPU() const 
	{
		return _ppu;
	}

private:
	void SystemClock();

private:
	const Cartridge& _cart;

	MemoryMap* _memoryCPU;
	Disassembler* _disassembler;
	CPU* _cpu;
	std::set<uint16_t> _breakpoints;

	PPUMemoryMap* _memoryPPU;
	PPU* _ppu;

	bool _continuousRun = false;
	bool _enableBreakpoints = false;
	bool _haltedBreakpoint = false;
	uint64_t _cycleCount = 0;
};


#pragma once
#include <cstdint>
#include <set>
#include "Memory/Cartridge.h"
#include "Memory/Mapper0.h"
#include "CPU/Disassembler.h"
#include "CPU/CPU.h"
#include "PPU/PPU.h"
#include "IO/IOPort.h"
#include "IO/StandardJoypad.h"

class System
{
public:

	System(Cartridge& cart);
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

	const IOPort* GetOutputPort()
	{
		return &_outputPort;
	}

	// maybe make these const ref and throw except instead of nullptr
	const IOPort* GetInputPort(int port)
	{
		if (port > 1) return nullptr;
		return &_inputPort[port];
	}

	StandardJoypad* GetJoypad(int joypad)
	{
		if (joypad > 1) return nullptr;
		return &_joypad[joypad];
	}

	void ChangeCartridge(Cartridge& cart);

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
	Cartridge& _cart;

	CPUMemoryMap* _memoryCPU = nullptr;
	Disassembler* _disassembler = nullptr;
	CPU* _cpu = nullptr;
	std::set<uint16_t> _breakpoints;

	PPUMemoryMap* _memoryPPU = nullptr;
	PPU* _ppu = nullptr;

	IOPort _outputPort = IOPort(PortDirection::Output);
	IOPort _inputPort[2] = { IOPort(PortDirection::Input), IOPort(PortDirection::Input) };
	StandardJoypad _joypad[2] = { StandardJoypad(_inputPort[0]), StandardJoypad(_inputPort[1])};

	int _mapper = 0;
	bool _continuousRun = false;
	bool _enableBreakpoints = false;
	bool _haltedBreakpoint = false;
	uint64_t _cycleCount = 0;
};


#include "System.h"

System::System(const Cartridge& cart)
	: _cart(cart)
{
	switch (_cart.Mapper())
	{
	case 0:
		_memoryPPU = new PPUMapper0(_cart.CharacterROM(), _cart.MirroringMode());
		_ppu = new PPU(*_memoryPPU);

		_memoryCPU = new CPUMapper0(_ppu, _cart.ProgramROM(), _cart.ProgramROMSize());
		_disassembler = new Disassembler(*_memoryCPU);
		_cpu = new CPU(*_memoryCPU, _disassembler);
	}
}

System::~System()
{
	delete _memoryCPU;
	delete _disassembler;
	delete _cpu;
	delete _memoryPPU;
	delete _ppu;
}

void System::ClockStep()
{
	if (!_continuousRun)
	{
		SystemClock();
	}
}

void System::InstructionStep()
{
	if (!_continuousRun)
	{
		while (_cpu->IsInstFinished())
		{
			SystemClock();
		}

		while (!_cpu->IsInstFinished())
		{
			SystemClock();
		}
	}
}

void System::SystemClock()
{
	_ppu->Clock();
	
	if (++_cycleCount % 3 == 0)
	{
		_cpu->Clock();
	}
}


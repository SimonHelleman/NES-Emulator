#include "System.h"

System::System(Cartridge& cart)
	: _cart(cart)
{
	_mapper = _cart.Mapper();
	switch (_mapper)
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

void System::ChangeCartridge(Cartridge& cart)
{
	_cart = cart;
	if (_cart.Mapper() == _mapper)
	{
		_memoryCPU->SetProgramROM(cart.ProgramROM(), cart.ProgramROMSize());
		_memoryPPU->SetCharacterROM(cart.CharacterROM());
		_memoryPPU->SetMirroringMode(cart.MirroringMode());
	}
	Reset();
}

void System::Reset()
{
	_disassembler->Clear();
	_cpu->Reset();
	_ppu->Reset();
}

void System::Update()
{
	if (_continuousRun)
	{
		if (_enableBreakpoints)
		{
			const auto& search = _breakpoints.find(_cpu->GetProgramCounter());
			if (search != _breakpoints.end() && _cpu->GetCurrentState() == CPU::State::Fetch)
			{
				if (!_haltedBreakpoint)
				{
					_continuousRun = false;
					_haltedBreakpoint = true;
				}
			}
		}
		SystemClock();
	}
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

void System::FrameStep()
{
	if (!_continuousRun)
	{
		while (_ppu->IsFrameComplete())
		{
			SystemClock();
		}

		while (!_ppu->IsFrameComplete())
		{
			SystemClock();
		}
	}
}

void System::AddBreakpoint(uint16_t addr)
{
	_breakpoints.insert(addr);
}

void System::RemoveBreakpoint(uint16_t addr)
{
	_breakpoints.erase(addr);
}

const std::set<uint16_t>& System::GetBreakpoints() const
{
	return _breakpoints;
}

void System::SystemClock()
{
	_ppu->Clock();
	
	if (++_cycleCount % 3 == 0)
	{
		_haltedBreakpoint = false;
		_cpu->Clock();
	}

	if (_ppu->PendingNMI())
	{
		_cpu->NMI();
		_ppu->ClearNMI();
	}
}


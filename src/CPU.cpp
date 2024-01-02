#include "CPU.h"

void CPU::Clock()
{
	if (_currentState == State::Fetch)
	{
		FetchInstruction();
		_currentState = State::AddressingMode;

		if (_doIRQ)
		{
			_currentInstruction.clockCycles += 7;	
		}
		
		if (_doNMI)
		{
			_currentInstruction.clockCycles += 8;
		}

		// At this point the inturrupt is completely
		// set up. So clear the flag
		if (_doIRQ && _isInstFinished)
		{
			_doIRQ = false;
		}
		
		if (_doNMI && _isInstFinished)
		{
			_doNMI = false;
		}

		_isInstFinished = false;
	}

	if (_currentState == State::AddressingMode)
	{
		_currentInstruction.addrModeOp(this);
		_currentState = State::Execute;
	}

	if (_currentState == State::Execute)
	{
		if (_currentInstruction.clockCycles > 0)
		{
			if (--_currentInstruction.clockCycles != 0)
			{
				return;
			}
		}

		_currentInstruction.operation(this);
		_currentState = State::Fetch;
		_isInstFinished = true;

		if (_doNMI)
		{
			HandleNMI();
		}

		AddToDisassembly();
	}
}

void CPU::FetchInstruction()
{
	uint8_t opcode = _memory.Read(_regPC++);

	// The copy is intentional since operations
	// that need more than one clock cycle will modify
	// the cycle count
	_currentInstruction = _opcodeMatrix[opcode];
}

void CPU::Reset()
{
	_regStatus = 0;
	_regStatus = STATUS_5;
	_regA = 0;
	_regX = 0;
	_regY = 0;
	_regSP = 0xfd;
	_regPC = RESET_VECTOR;
	Absolute();
	_regPC = _currentAddr;
	_currentState = State::Fetch;

	AddToDisassembly();
}

void CPU::IRQ()
{
	_doIRQ = !(_regStatus | STATUS_I);

	if (_isInstFinished)
	{
		HandleIRQ();
		AddToDisassembly();
	}
}

void CPU::NMI()
{
	if (_isInstFinished)
	{
		HandleNMI();
		AddToDisassembly();
	}
	_doNMI = true;
}

void CPU::HandleIRQ()
{
	// Push return address to stack
	_memory.Write(0x0100 | _regSP--, _regPC >> 8);
	_memory.Write(0x0100 | _regSP--, _regPC & 0b11111111);

	_memory.Write(0x0100 | _regSP--, _regStatus);

	_regPC = IRQ_VECTOR;
	Absolute();
	_regPC = _currentAddr;
	
}

void CPU::HandleNMI()
{
	// Push return address to stack
	_memory.Write(0x0100 | _regSP--, _regPC >> 8);
	_memory.Write(0x0100 | _regSP--, _regPC & 0b11111111);

	_memory.Write(0x0100 | _regSP--, _regStatus);

	_regPC = NMI_VECTOR;
	Absolute();
	_regPC = _currentAddr;
}

void CPU::AddToDisassembly()
{
	if (_disassembler != nullptr)
	{
		Opcode inst = _opcodeMatrix[_memory.Read(_regPC, true)];
		_disassembler->AddInstruction(_regPC, inst.mnemonic, inst.size, inst.addrMode);
	}
}

void CPU::BRK()
{
	HandleIRQ();
	_regStatus |= STATUS_B;
}

void CPU::JSR()
{
	// Push return address to stack
	_memory.Write(0x0100 | _regSP--, (--_regPC) >> 8);
	_memory.Write(0x0100 | _regSP--, _regPC & 0b11111111);

	_regPC = _currentAddr;
}

void CPU::RTI()
{
	_regStatus = _memory.Read(0x100 | ++_regSP);
	uint8_t retAddrLow = _memory.Read(0x0100 | ++_regSP);
	uint8_t retAddrHigh = _memory.Read(0x0100 | ++_regSP);

	_regStatus &= ~STATUS_B;
	_regPC = retAddrHigh << 8 | retAddrLow;
}

void CPU::RTS()
{
	uint8_t retAddrLow = _memory.Read(0x0100 | ++_regSP);
	uint8_t retAddrHigh = _memory.Read(0x0100 | ++_regSP);

	_regPC = (retAddrHigh << 8 | retAddrLow) + 1;
}

void CPU::NOP()
{
}

void CPU::BIT()
{
	uint8_t target = _memory.Read(_currentAddr);
	_regStatus = (_regA & target) == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = target & 0b01000000 ? _regStatus | STATUS_V : _regStatus & ~STATUS_V;
	_regStatus = target & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::STY()
{
	_memory.Write(_currentAddr, _regY);
}

void CPU::LDY()
{
	_regY = _memory.Read(_currentAddr);

	_regStatus = _regY == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regY & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::CPY()
{
	uint8_t target = _memory.Read(_currentAddr);

	_regStatus = _regY >= target ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = _regY == target ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = (_regY - target) & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::CPX()
{
	uint8_t target = _memory.Read(_currentAddr);

	_regStatus = _regX >= target ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = _regX == target ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = (_regX - target) & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::PHP()
{
	_memory.Write(0x0100 | _regSP--, _regStatus);
}

void CPU::PLP()
{
	_regStatus = _memory.Read(0x0100 | ++_regSP);
}

void CPU::PHA()
{
	_memory.Write(0x0100 | _regSP--, _regA);
}

void CPU::PLA()
{
	_regA = _memory.Read(0x0100 | ++_regSP);
	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::DEY()
{
	--_regY;

	_regStatus = _regY == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regY & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::TAY()
{
	_regY = _regA;

	_regStatus = _regY == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regY & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::INY()
{	
	++_regY;

	_regStatus = _regY == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regY & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::INX()
{
	++_regX;

	_regStatus = _regX == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regX & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::JMP()
{
	_regPC = _currentAddr;
}

void CPU::BPL()
{
	if (!(_regStatus & STATUS_N))
	{
		_regPC += _branchOffset;
	}
}

void CPU::BMI()
{
	if (_regStatus & STATUS_N)
	{
		_regPC += _branchOffset;
	}
}

void CPU::BVC()
{
	if (!(_regStatus & STATUS_V))
	{
		_regPC += _branchOffset;
	}
}

void CPU::BVS()
{
	if (_regStatus & STATUS_V)
	{
		_regPC += _branchOffset;
	}
}

void CPU::BCC()
{
	if (!(_regStatus & STATUS_C))
	{
		_regPC += _branchOffset;
	}
}

void CPU::BCS()
{
	if (_regStatus & STATUS_C)
	{
		_regPC += _branchOffset;
	}
}

void CPU::BNE()
{
	if (!(_regStatus & STATUS_Z))
	{
		_regPC += _branchOffset;
	}
}

void CPU::BEQ()
{
	if (_regStatus & STATUS_Z)
	{
		_regPC += _branchOffset;
	}
}

void CPU::CLC()
{
	_regStatus &= ~STATUS_C;
}

void CPU::SEC()
{
	_regStatus |= STATUS_C;
}

void CPU::CLI()
{
	_regStatus &= ~STATUS_I;
}

void CPU::SEI()
{
	_regStatus |= STATUS_I;
}

void CPU::TYA()
{
	_regA = _regY;

	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::CLV()
{
	_regStatus &= ~STATUS_V;
}

void CPU::CLD()
{
	_regStatus &= ~STATUS_D;
}

void CPU::SED()
{
	_regStatus |= STATUS_D;
}

void CPU::ORA()
{
	_regA |= _memory.Read(_currentAddr);
	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::AND()
{
	_regA &= _memory.Read(_currentAddr);
	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::EOR()
{
	_regA ^= _memory.Read(_currentAddr);
	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::ADC()
{
	int8_t oldA = static_cast<int8_t>(_regA);
	int8_t addend = static_cast<int8_t>(_memory.Read(_currentAddr));
	_regA += static_cast<uint8_t>(addend) + ((_regStatus & STATUS_C) ? 1 : 0);

	_regStatus = _regA < static_cast<uint8_t>(oldA) ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	
	// Overflow:
	//     Case 1 -> neg + neg = pos (both sign bits set but sum sign bit not set) 
	//     Case 2 -> pos + pos = neg (both sign bits not set but sum sign bit set)
	bool overflowCase1 = oldA < 0 && addend < 0 && static_cast<int8_t>(_regA) > 0;
	bool overflowCase2 = oldA > 0 && addend > 0 && static_cast<int8_t>(_regA) < 0;
	_regStatus = overflowCase1 || overflowCase2 ? _regStatus | STATUS_V : _regStatus & ~STATUS_V;

	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::STA()
{
	_memory.Write(_currentAddr, _regA);
}

void CPU::LDA()
{
	_regA = _memory.Read(_currentAddr);
	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::CMP()
{
	uint8_t target = _memory.Read(_currentAddr);

	_regStatus = _regA >= target ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = _regA == target ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = (_regA - target) & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::SBC()
{
	int8_t oldA = static_cast<int8_t>(_regA);
	int8_t subtrahend = static_cast<int8_t>(_memory.Read(_currentAddr));
	_regA -= static_cast<uint8_t>(subtrahend) + ((_regStatus & STATUS_C) ? 0 : 1);

	_regStatus = _regA < static_cast<uint8_t>(oldA) ? _regStatus & ~STATUS_C : _regStatus | STATUS_C;
	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;

	bool overflowCase1 = oldA < 0 && subtrahend < 0 && static_cast<int8_t>(_regA) > 0;
	bool overflowCase2 = oldA > 0 && subtrahend > 0 && static_cast<int8_t>(_regA) < 0;
	_regStatus = overflowCase1 || overflowCase2 ? _regStatus | STATUS_V : _regStatus & ~STATUS_V;

	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::LDX()
{
	_regX = _memory.Read(_currentAddr);

	_regStatus = _regX == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regX & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::ASL()
{
	uint8_t val = _isAccumOpcode ? _regA : _memory.Read(_currentAddr);
	_regStatus = val & 0b10000000 ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = val == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;

	val <<= 1;

	_regStatus = val & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;

	if (_isAccumOpcode)
	{
		_regA = val;
		_isAccumOpcode = false;
		return;
	}

	_memory.Write(_currentAddr, val);
}

void CPU::ROL()
{
	uint8_t val = _isAccumOpcode ? _regA : _memory.Read(_currentAddr);

	_regStatus = val & 0b10000000 ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = val == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;

	val <<= 1;

	val |= _regStatus & STATUS_C;
	_regStatus = val & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;

	if (_isAccumOpcode)
	{
		_regA = val;
		_isAccumOpcode = false;
		return;
	}

	_memory.Write(_currentAddr, val);
}

void CPU::LSR()
{
	uint8_t val = _isAccumOpcode ? _regA : _memory.Read(_currentAddr);

	_regStatus = val & 0b00000001 ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = val == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;

	val >>= 1;

	_regStatus = val & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;

	if (_isAccumOpcode)
	{
		_regA = val;
		_isAccumOpcode = false;
		return;
	}

	_memory.Write(_currentAddr, val);
}

void CPU::ROR()
{
	uint8_t val = _isAccumOpcode ? _regA : _memory.Read(_currentAddr);

	_regStatus = val & 0b00000001 ? _regStatus | STATUS_C : _regStatus & ~STATUS_C;
	_regStatus = val == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;

	val >>= 1;
	val |= (_regStatus & STATUS_C) << 7;
	_regStatus = val & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;

	if (_isAccumOpcode)
	{
		_regA = val;
		_isAccumOpcode = false;
		return;
	}

	_memory.Write(_currentAddr, val);
}

void CPU::STX()
{
	_memory.Write(_currentAddr, _regX);
}

void CPU::DEC()
{
	uint8_t target = _memory.Read(_currentAddr);

	_memory.Write(_currentAddr, --target);

	_regStatus = target == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = target & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::INC()
{
	uint8_t target = _memory.Read(_currentAddr);
	
	_memory.Write(_currentAddr, ++target);

	_regStatus = target == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = target & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::TXA()
{
	_regA = _regX;

	_regStatus = _regA == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regA & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::TAX()
{
	_regX = _regA;

	_regStatus = _regX == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regX & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::DEX()
{
	--_regX;

	_regStatus = _regX == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regX & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::TXS()
{
	_regSP = _regX;
}

void CPU::TSX()
{
	_regX = _regSP;
	_regStatus = _regX == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;
	_regStatus = _regX & 0b10000000 ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::SLO()
{
}

void CPU::RLA()
{
}

void CPU::SRE()
{
}

void CPU::RRA()
{
}

void CPU::SAX()
{
}

void CPU::LAX()
{
}

void CPU::DCP()
{
}

void CPU::ISC()
{
}

void CPU::ANC()
{
}

void CPU::ALR()
{
}

void CPU::ARR()
{
}

void CPU::AXS()
{
}

void CPU::Accumulator()
{
	_isAccumOpcode = true;
}

void CPU::Immediate()
{
	_currentAddr = _regPC++;
}

void CPU::ZeroPage()
{
	_currentAddr = _memory.Read(_regPC++);
}

void CPU::ZeroPageIndexedX()
{
	_currentAddr = _memory.Read(_regPC++) + _regX;
}

void CPU::ZeroPageIndexedY()
{
	_currentAddr = _memory.Read(_regPC++) + _regY;
}

void CPU::Absolute()
{
	uint8_t addrLow = _memory.Read(_regPC++);
	uint8_t addrHigh = _memory.Read(_regPC++);

	_currentAddr = addrHigh << 8 | addrLow;
}

void CPU::AbsoluteIndexedX()
{
	uint8_t addrLow = _memory.Read(_regPC++);
	uint8_t addrHigh = _memory.Read(_regPC++);

	_currentAddr = (addrHigh << 8 | addrLow) + _regX;

	// Crossed a page boundary
	if ((_currentAddr & 0xff00) >> 8 != addrHigh)
	{
		++(_currentInstruction.clockCycles);
	}
}

void CPU::AbsoluteIndexedY()
{
	uint8_t addrLow = _memory.Read(_regPC++);
	uint8_t addrHigh = _memory.Read(_regPC++);

	_currentAddr = (addrHigh << 8 | addrLow) + _regY;

	// Crossed a page boundary, add the oops cycle
	if ((_currentAddr & 0xff00) >> 8 != addrHigh)
	{
		++(_currentInstruction.clockCycles);
	}
}

void CPU::Relative()
{
	_branchOffset = _memory.Read(_regPC++);
}

void CPU::Indirect()
{
	uint8_t ptrLow = _memory.Read(_regPC++);
	uint8_t ptrHigh = _memory.Read(_regPC++);

	uint8_t addrLow = _memory.Read(ptrLow);
	uint8_t addrHigh = _memory.Read(ptrHigh);

	_currentAddr = (addrHigh << 8) | addrLow;
}

void CPU::IndexedIndirectX()
{
	uint8_t ptrLow = _memory.Read(_regPC++);

	uint8_t addrLow = _memory.Read(ptrLow + _regX & 0x00ff);
	uint8_t addrHigh = _memory.Read(ptrLow + _regX + 1 & 0x00ff);

	_currentAddr = (addrHigh << 8) | addrLow;
}

void CPU::IndirectIndexedY()
{
	uint16_t ptrLow = _memory.Read(_regPC++);

	uint8_t addrLow = _memory.Read(ptrLow & 0x00ff);
	uint8_t addrHigh = _memory.Read((ptrLow + 1) & 0x00ff);

	_currentAddr = ((addrHigh << 8) | addrLow) + _regY;
}
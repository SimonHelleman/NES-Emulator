#pragma once
#include <cstdint>
#include <memory>
#include <functional>
#include "Memory/MemoryMap.h"
#include "Disassembler.h"

class CPU
{
public:
	CPU(MemoryMap& memory, Disassembler* disassembler)
		: _memory(memory), _opcodeMatrix(CreateOpcodeMatrix()), _disassembler(disassembler)
	{
	}

	void Clock();
	void FetchInstruction();
	void Reset();

	void IRQ();
	void NMI();

	struct Opcode
	{
		const char* mnemonic;
		AdressingMode addrMode;
		std::function<void(CPU*)> addrModeOp;
		std::function<void(CPU*)> operation;
		int clockCycles;
		int size;
	};

	enum class State
	{
		Fetch,
		AddressingMode,
		Execute
	};

	const Opcode& GetCurrentInstruction() const
	{
		return _currentInstruction;
	}

	State GetCurrentState() const
	{
		return _currentState;
	}

	uint16_t GetProgramCounter() const
	{
		return _regPC;
	}

	uint8_t GetA() const
	{
		return _regA;
	}

	uint8_t GetX() const
	{
		return _regX;
	}

	uint8_t GetY() const
	{
		return _regY;
	}

	uint8_t GetSP() const
	{
		return _regSP;
	}

	uint8_t GetStatusReg() const
	{
		return _regStatus;
	}

	bool IsInstFinished() const
	{
		return _isInstFinished;
	}

	std::unique_ptr<Opcode[]> CreateOpcodeMatrix();

private:
	void HandleIRQ();
	void HandleNMI();

	void AddToDisassembly();

// CPU instructions
private:

	void BRK();
	void JSR();
	void RTI();
	void RTS();
	void NOP();
	void LDY();
	void CPY();
	void CPX();

	void BIT();
	void STY();
	
	void PHP();
	void PLP();
	void PHA();
	void PLA();
	void DEY();
	void TAY();
	void INY();
	void INX();

	void JMP();

	void BPL();
	void BMI();
	void BVC();
	void BVS();
	void BCC();
	void BCS();
	void BNE();
	void BEQ();

	void CLC();
	void SEC();
	void CLI();
	void SEI();
	void TYA();
	void CLV();
	void CLD();
	void SED();

	void ORA();
	void AND();
	void EOR();
	void ADC();
	void STA();
	void LDA();
	void CMP();
	void SBC();

	void LDX();
	
	void ASL();
	void ROL();
	void LSR();
	void ROR();
	void STX();
	void DEC();
	void INC();

	void TXA();
	void TAX();
	void DEX();

	void TXS();
	void TSX();

	void SLO();
	void RLA();
	void SRE();
	void RRA();
	void SAX();
	void LAX();
	void DCP();
	void ISC();

	void ANC();
	void ALR();
	void ARR();
	void AXS();

private:

	// CPU Addressing Modes
	void Accumulator();
	void Immediate();
	void ZeroPage();
	void ZeroPageIndexedX();
	void ZeroPageIndexedY();
	void Absolute();
	void AbsoluteIndexedX();
	void AbsoluteIndexedY();
	void Relative();
	void Indirect();
	void IndexedIndirectX();
	void IndirectIndexedY();
	void Implied() { }

public:
	static constexpr uint8_t STATUS_N = 0b10000000;
	static constexpr uint8_t STATUS_V = 0b01000000;
	static constexpr uint8_t STATUS_5 = 0b00100000;
	static constexpr uint8_t STATUS_B = 0b00010000;
	static constexpr uint8_t STATUS_D = 0b00001000; // NES didn't have decimal mode -> not implementing
	static constexpr uint8_t STATUS_I = 0b00000100;
	static constexpr uint8_t STATUS_Z = 0b00000010;
	static constexpr uint8_t STATUS_C = 0b00000001;

private:
	MemoryMap& _memory;
	Disassembler* _disassembler;

	// CPU registers
	uint8_t _regX = 0;
	uint8_t _regY = 0;
	uint8_t _regA = 0;
	uint8_t _regSP = 0;
	uint8_t _regStatus = 0;
	uint16_t _regPC = 0;

	bool _doIRQ = false;
	bool _doNMI = false;
	bool _isAccumOpcode = false;
	bool _isInstFinished = false;

	uint16_t _currentAddr = 0;
	int8_t _branchOffset = 0;
	Opcode _currentInstruction = { 0 };
	State _currentState = State::Fetch;

	std::unique_ptr<Opcode[]> _opcodeMatrix;

	static constexpr uint16_t NMI_VECTOR = 0xfffa;
	static constexpr uint16_t RESET_VECTOR = 0xfffc;
	static constexpr uint16_t IRQ_VECTOR = 0xfffe;
};


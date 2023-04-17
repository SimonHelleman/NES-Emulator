#pragma once
#include <cstdint>
#include <memory>
#include <functional>
#include "MemoryMap.h"

class CPU
{
public:
	CPU(MemoryMap& memory);

	void Clock();
	void FetchInstruction();
	void Reset();

	void TriggerIRQ();
	void TriggerNMI();

	static struct Opcode
	{
		const char* mnemonic;
		std::function<void(CPU*)> addrMode;
		std::function<void(CPU*)> operation;
		int clockCycles;
	};

private:
	static enum class State
	{
		Ready,
		AddressingMode,
		Execute
	};

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


private:

	// CPU registers
	uint8_t _regX;
	uint8_t _regY;
	uint8_t _regA;
	uint8_t _regSP;
	uint8_t _regStatus;
	uint16_t _regPC;

	MemoryMap& _memory;

	uint16_t _currentAddr;
	int8_t _branchOffset;
	Opcode _currentInstruction;
	State _currentState = State::READY;
	std::unique_ptr<Opcode[]> _opcodeMatrix;

	static constexpr uint8_t STATUS_N = 0b10000000;
	static constexpr uint8_t STATUS_V = 0b01000000;
	static constexpr uint8_t STATUS_5 = 0b00100000;
	static constexpr uint8_t STATUS_B = 0b00010000;
	static constexpr uint8_t STATUS_D = 0b00001000; // NES didn't have decimal mode -> not implementing
	static constexpr uint8_t STATUS_I = 0b00000100;
	static constexpr uint8_t STATUS_Z = 0b00000010;
	static constexpr uint8_t STATUS_C = 0b00000001;

	static constexpr uint16_t RESET_VECTOR = 0xfffc;
	static constexpr uint16_t IRQ_VECTOR = 0xfffe;
};


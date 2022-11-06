#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include "Bus.h"

class CPU
{
public:
	CPU(Bus& bus)
		: _bus(bus)
	{}

	void Clock();

	void Fetch();

public:
	struct Instruction
	{
		std::string name;
		std::function<void()> func;
		std::function<uint8_t()> addrMode;
		int nCycles;
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

	void SHY();

	void ORA();
	void AND();
	void EOR();
	void ADC();
	void STA();
	void LDA();
	void CMP();
	void SBC();

	void STP();
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

	void SHX();

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
	void XAA();
	void AXS();

	void AHX();
	
	void TAS();
	void LAS();

private:

	// CPU registers
	uint8_t _regX;
	uint8_t _regY;
	uint8_t _regA;
	uint8_t _regSP;
	uint8_t _regPC;
	uint8_t _regStatus;

private:
	Bus& _bus;

private:
	static constexpr uint8_t STATUS_N = 0b10000000;
	static constexpr uint8_t STATUS_V = 0b01000000;
	static constexpr uint8_t STATUS_5 = 0b00100000;
	static constexpr uint8_t STATUS_4 = 0b00010000;
	static constexpr uint8_t STATUS_D = 0b00001000;
	static constexpr uint8_t STATUS_I = 0b00000100;
	static constexpr uint8_t STATUS_Z = 0b00000010;
	static constexpr uint8_t STATUS_C = 0b00000001;
public:
	static constexpr uint16_t RESET_VECTOR = 0xfffc;

};


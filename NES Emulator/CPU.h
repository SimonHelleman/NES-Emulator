#pragma once
#include <cstdint>

class CPU
{
public:
	CPU();

	void clock();

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

	void NOP();
	void BIT();
	void STY();
	void LDY();
	void CPY();
	void CPX();
	
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
	void SBC();

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

public:
	static constexpr uint16_t RESET_VECTOR = 0xfffc;

};


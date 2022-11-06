#include "CPU.h"


void CPU::BRK()
{
}

void CPU::JSR()
{
}

void CPU::RTI()
{
}

void CPU::RTS()
{
}

void CPU::NOP()
{
}

void CPU::BIT()
{
}

void CPU::STY()
{
}

void CPU::LDY()
{
}

void CPU::CPY()
{
}

void CPU::CPX()
{
}

void CPU::PHP()
{
}

void CPU::PLP()
{
}

void CPU::PHA()
{
}

void CPU::PLA()
{
}

void CPU::DEY()
{
}

void CPU::TAY()
{
}

void CPU::INY()
{	
	_regStatus = ++_regY == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;

	_regStatus = (_regY & 0b10000000) ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::INX()
{
	_regStatus = ++_regX == 0 ? _regStatus | STATUS_Z : _regStatus & ~STATUS_Z;

	_regStatus = (_regX & 0b10000000) ? _regStatus | STATUS_N : _regStatus & ~STATUS_N;
}

void CPU::JMP()
{
}

void CPU::BPL()
{
}

void CPU::BMI()
{
}

void CPU::BVC()
{
}

void CPU::BVS()
{
}

void CPU::BCC()
{
}

void CPU::BCS()
{
}

void CPU::BNE()
{
}

void CPU::BEQ()
{
}

void CPU::CLC()
{
}

void CPU::SEC()
{
}

void CPU::CLI()
{
}

void CPU::SEI()
{
}

void CPU::TYA()
{
}

void CPU::CLV()
{
}

void CPU::CLD()
{
}

void CPU::SED()
{
}

void CPU::SHY()
{
}

void CPU::ORA()
{
}

void CPU::AND()
{
}

void CPU::EOR()
{
}

void CPU::ADC()
{
}

void CPU::STA()
{
}

void CPU::LDA()
{
}

void CPU::CMP()
{
}

void CPU::SBC()
{
}

void CPU::AHX()
{
}

void CPU::STP()
{
}

void CPU::LDX()
{
}

void CPU::ASL()
{
}

void CPU::ROL()
{
}

void CPU::LSR()
{
}

void CPU::ROR()
{
}

void CPU::STX()
{
}

void CPU::DEC()
{
}

void CPU::INC()
{
}

void CPU::TXA()
{
}

void CPU::TAX()
{
}

void CPU::DEX()
{
}

void CPU::TXS()
{
}

void CPU::TSX()
{
}

void CPU::SHX()
{
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

void CPU::XAA()
{
}

void CPU::AXS()
{
}

void CPU::TAS()
{
}

void CPU::LAS()
{
}

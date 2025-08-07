#include "CPU.h"

std::unique_ptr<CPU::Opcode[]> CPU::CreateOpcodeMatrix()
{
	std::unique_ptr<CPU::Opcode[]> ret = std::make_unique<CPU::Opcode[]>(256);

	// Not implmented:
	// STP: Unofficial opcode that puts the CPU into a low power state until a
	//      hardware interrupt occurs. No games use this apparently.
	// XAA: Unofficial opcode that has unpredictable behaviour which depends on analog effects.
	// AHX: Unofficial opcode that has no documentation
	// TAS: Unofficial opcode that has no documentation
	// SHY: Unofficial opcode that has no documentation
	// SHX: Unofficial opthat has no documentation
	// LAS: Unofficial opthat has no documentation
	ret[0x00] = { "BRK", AdressingMode::Implied, &CPU::Implied, &CPU::BRK, 7, 1 };
	ret[0x01] = { "ORA", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::ORA, 6, 2 };
	ret[0x02] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x03] = { "SLO", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::SLO, 8, 2 };
	ret[0x04] = { "NOP", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::NOP, 4, 2 };
	ret[0x05] = { "ORA", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::ORA, 3, 2 };
	ret[0x06] = { "ASL", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::ASL, 5, 2 };
	ret[0x07] = { "SLO", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::SLO, 5, 2 };
	ret[0x08] = { "PHP", AdressingMode::Implied, &CPU::Implied, &CPU::PHP, 3, 1 };
	ret[0x09] = { "ORA", AdressingMode::Immediate, &CPU::Immediate, &CPU::ORA, 2, 2 };
	ret[0x0a] = { "ASL", AdressingMode::Accumulator, &CPU::Accumulator, &CPU::ASL, 2, 1 };
	ret[0x0b] = { "ANC", AdressingMode::Immediate, &CPU::Immediate, &CPU::ANC, 2, 2 };
	ret[0x0c] = { "NOP", AdressingMode::Absolute, &CPU::Absolute, &CPU::NOP, 4, 3 };
	ret[0x0d] = { "ORA", AdressingMode::Absolute, &CPU::Absolute, &CPU::ORA, 4, 3 };
	ret[0x0e] = { "ASL", AdressingMode::Absolute, &CPU::Absolute, &CPU::ASL, 6, 3 };
	ret[0x0f] = { "SLO", AdressingMode::Absolute, &CPU::Absolute, &CPU::SLO, 6, 3 };

	ret[0x10] = { "BPL", AdressingMode::Relative, &CPU::Relative, &CPU::BPL, 2, 2 };
	ret[0x11] = { "ORA", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::ORA, 5, 2 };
	ret[0x12] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x13] = { "SLO", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::SLO, 8, 2 };
	ret[0x14] = { "NOP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	ret[0x15] = { "ORA", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::ORA, 4, 2 };
	ret[0x16] = { "ASL", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::ASL, 6, 2 };
	ret[0x17] = { "SLO", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::SLO, 6, 2 };
	ret[0x18] = { "CLC", AdressingMode::Implied, &CPU::Implied, &CPU::CLC, 2, 1 };
	ret[0x19] = { "ORA", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::ORA, 4, 3 };
	ret[0x1a] = { "NOP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 2, 1 };
	ret[0x1b] = { "SLO", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::SLO, 7, 3 };
	ret[0x1c] = { "NOP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::NOP, 4, 3 }; // aka IGN
	ret[0x1d] = { "ORA", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::ORA, 4, 3 };
	ret[0x1e] = { "ASL", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::ASL, 7, 3 };
	ret[0x1f] = { "SLO", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::SLO, 7, 3 };

	ret[0x20] = { "JSR", AdressingMode::Absolute, &CPU::Absolute, &CPU::JSR, 6, 3 };
	ret[0x21] = { "AND", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::AND, 6, 2 };
	ret[0x22] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x23] = { "RLA", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::RLA, 8, 2 };
	ret[0x24] = { "BIT", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::BIT, 3, 2 };
	ret[0x25] = { "AND", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::AND, 3, 2 };
	ret[0x26] = { "ROL", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::ROL, 5, 2 };
	ret[0x27] = { "RLA", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::RLA, 5, 2 };
	ret[0x28] = { "PLP", AdressingMode::Implied, &CPU::Implied, &CPU::PLP, 4, 1 };
	ret[0x29] = { "AND", AdressingMode::Immediate, &CPU::Immediate, &CPU::AND, 2, 2 };
	ret[0x2a] = { "ROL", AdressingMode::Accumulator, &CPU::Accumulator, &CPU::ROL, 2, 1 };
	ret[0x2b] = { "ANC", AdressingMode::Immediate, &CPU::Immediate, &CPU::ANC, 2, 2 };
	ret[0x2c] = { "BIT", AdressingMode::Absolute, &CPU::Absolute, &CPU::BIT, 4, 3 };
	ret[0x2d] = { "AND", AdressingMode::Absolute, &CPU::Absolute, &CPU::AND, 4, 3 };
	ret[0x2e] = { "ROL", AdressingMode::Absolute, &CPU::Absolute, &CPU::ROL, 6, 3 };
	ret[0x2f] = { "RLA", AdressingMode::Absolute, &CPU::Absolute, &CPU::RLA, 6, 3 };

	ret[0x30] = { "BMI", AdressingMode::Relative, &CPU::Relative, &CPU::BMI, 2, 2 };
	ret[0x31] = { "AND", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::AND, 5, 2 };
	ret[0x32] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 1, 1 };
	ret[0x33] = { "RLA", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::RLA, 8, 2 };
	ret[0x34] = { "NOP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	ret[0x35] = { "AND", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::AND, 4, 2 };
	ret[0x36] = { "ROL", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::ROL, 6, 2 };
	ret[0x37] = { "RLA", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::RLA, 6, 2 };
	ret[0x38] = { "SEC", AdressingMode::Implied, &CPU::Implied, &CPU::SEC, 2, 1 };
	ret[0x39] = { "AND", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::AND, 4, 3 };
	ret[0x3a] = { "NOP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 2, 1 };
	ret[0x3b] = { "RLA", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::RLA, 7, 3 };
	ret[0x3c] = { "NOP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::NOP, 4, 3 };
	ret[0x3d] = { "AND", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::AND, 4, 3 };
	ret[0x3e] = { "ROL", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::ROL, 7, 3 };
	ret[0x3f] = { "RLA", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::RLA, 7, 3 };

	ret[0x40] = { "RTI", AdressingMode::Implied, &CPU::Implied, &CPU::RTI, 6, 1 };
	ret[0x41] = { "EOR", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::EOR, 6, 2 };
	ret[0x42] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x43] = { "SRE", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::SRE, 8, 2 };
	ret[0x44] = { "NOP", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::NOP, 3, 2 }; // aka IGN
	ret[0x45] = { "EOR", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::EOR, 3, 2 };
	ret[0x46] = { "LSR", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::LSR, 5, 2 };
	ret[0x47] = { "SRE", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::SRE, 5, 2 };
	ret[0x48] = { "PHA", AdressingMode::Implied, &CPU::Implied, &CPU::PHA, 3, 1 };
	ret[0x49] = { "EOR", AdressingMode::Immediate, &CPU::Immediate, &CPU::EOR, 2, 2 };
	ret[0x4a] = { "LSR", AdressingMode::Accumulator, &CPU::Accumulator, &CPU::LSR, 2, 1 };
	ret[0x4b] = { "ALR", AdressingMode::Immediate, &CPU::Immediate, &CPU::ALR, 2, 2 };
	ret[0x4c] = { "JMP", AdressingMode::Absolute, &CPU::Absolute, &CPU::JMP, 3, 3 };
	ret[0x4d] = { "EOR", AdressingMode::Absolute, &CPU::Absolute, &CPU::EOR, 4, 3 };
	ret[0x4e] = { "LSR", AdressingMode::Absolute, &CPU::Absolute, &CPU::LSR, 6, 3 };
	ret[0x4f] = { "SRE", AdressingMode::Absolute, &CPU::Absolute, &CPU::SRE, 6, 3 };

	ret[0x50] = { "BVC", AdressingMode::Relative, &CPU::Relative, &CPU::BVC, 2, 2 };
	ret[0x51] = { "EOR", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::EOR, 5, 2 };
	ret[0x52] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x53] = { "SRE", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::SRE, 8, 2 };
	ret[0x54] = { "NOP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	ret[0x55] = { "EOR", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::EOR, 4, 2 };
	ret[0x56] = { "LSR", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::LSR, 6, 2 };
	ret[0x57] = { "SRE", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::SRE, 6, 2 };
	ret[0x58] = { "CLI", AdressingMode::Implied, &CPU::Implied, &CPU::CLI, 2, 1 };
	ret[0x59] = { "EOR", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::EOR, 4, 3 };
	ret[0x5a] = { "NOP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 2, 1 };
	ret[0x5b] = { "SRE", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::SRE, 7, 3 };
	ret[0x5c] = { "NOP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	ret[0x5d] = { "EOR", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::EOR, 4, 3 };
	ret[0x5e] = { "LSR", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::LSR, 7, 3 };
	ret[0x5f] = { "SRE", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::SRE, 7, 3 };

	ret[0x60] = { "RTS", AdressingMode::Implied, &CPU::Implied, &CPU::RTS, 6, 1 };
	ret[0x61] = { "ADC", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::ADC, 6, 2 };
	ret[0x62] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x63] = { "RRA", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::RRA, 8, 2 };
	ret[0x64] = { "NOP", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::NOP, 3, 2 }; // aka IGN
	ret[0x65] = { "ADC", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::ADC, 3, 2 };
	ret[0x66] = { "ROR", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::ROR, 5, 2 };
	ret[0x67] = { "RRA", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::RRA, 5, 2 };
	ret[0x68] = { "PLA", AdressingMode::Implied, &CPU::Implied, &CPU::PLA, 4, 1 };
	ret[0x69] = { "ADC", AdressingMode::Immediate, &CPU::Immediate, &CPU::ADC, 2, 2 };
	ret[0x6a] = { "ROR", AdressingMode::Accumulator, &CPU::Accumulator, &CPU::ROR, 2, 1 }; // *
	ret[0x6b] = { "ARR", AdressingMode::Immediate, &CPU::Immediate, &CPU::ARR, 2, 2 };
	ret[0x6c] = { "JMP", AdressingMode::Indirect, &CPU::Indirect, &CPU::JMP, 5, 3 };
	ret[0x6d] = { "ADC", AdressingMode::Absolute, &CPU::Absolute, &CPU::ADC, 4, 3 };
	ret[0x6e] = { "ROR", AdressingMode::Absolute, &CPU::Absolute, &CPU::ROR, 6, 3 };
	ret[0x6f] = { "RRA", AdressingMode::Absolute, &CPU::Absolute, &CPU::RRA, 6, 3 };

	ret[0x70] = { "BVS", AdressingMode::Relative, &CPU::Relative, &CPU::BVS, 2, 2 };
	ret[0x71] = { "ADC", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::ADC, 5, 2 };
	ret[0x72] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x73] = { "RRA", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::RRA, 8, 2 };
	ret[0x74] = { "NOP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	ret[0x75] = { "ADC", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::ADC, 4, 2 };
	ret[0x76] = { "ROR", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::ROR, 6, 2 };
	ret[0x77] = { "RRA", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::RRA, 6, 2 };
	ret[0x78] = { "SEI", AdressingMode::Implied, &CPU::Implied, &CPU::SEI, 2, 1 };
	ret[0x79] = { "ADC", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::ADC, 4, 3 };
	ret[0x7a] = { "NOP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 2, 1 };
	ret[0x7b] = { "RRA", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::RRA, 7, 3 };
	ret[0x7c] = { "NOP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	ret[0x7d] = { "ADC", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::ADC, 4, 3 };
	ret[0x7e] = { "ROR", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::ROR, 7, 3 };
	ret[0x7f] = { "RRA", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::RRA, 7, 3 };

	ret[0x80] = { "NOP", AdressingMode::Immediate, &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SBK
	ret[0x81] = { "STA", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::STA, 6, 2 };
	ret[0x82] = { "NOP", AdressingMode::Immediate, &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	ret[0x83] = { "SAX", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::SAX, 6, 2 };
	ret[0x84] = { "STY", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::STY, 3, 2 };
	ret[0x85] = { "STA", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::STA, 3, 2 };
	ret[0x86] = { "STX", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::STX, 3, 2 };
	ret[0x87] = { "SAX", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::SAX, 3, 2 };
	ret[0x88] = { "DEY", AdressingMode::Implied, &CPU::Implied, &CPU::DEY, 2, 1 };
	ret[0x89] = { "NOP", AdressingMode::Immediate, &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	ret[0x8a] = { "TXA", AdressingMode::Implied, &CPU::Implied, &CPU::TXA, 2, 1 };
	ret[0x8b] = { "XAA", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x8c] = { "STY", AdressingMode::Absolute, &CPU::Absolute, &CPU::STY, 4, 3 };
	ret[0x8d] = { "STA", AdressingMode::Absolute, &CPU::Absolute, &CPU::STA, 4, 3 };
	ret[0x8e] = { "STX", AdressingMode::Absolute, &CPU::Absolute, &CPU::STX, 4, 3 };
	ret[0x8f] = { "SAX", AdressingMode::Absolute, &CPU::Absolute, &CPU::SAX, 4, 3 };

	ret[0x90] = { "BCC", AdressingMode::Relative, &CPU::Relative, &CPU::BCC, 2, 2 };
	ret[0x91] = { "STA", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::STA, 6, 2 };
	ret[0x92] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x93] = { "AHX", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x94] = { "STY", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::STY, 4, 2 };
	ret[0x95] = { "STA", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::STA, 4, 2 };
	ret[0x96] = { "STA", AdressingMode::ZeroPageIndexedY, &CPU::ZeroPageIndexedY, &CPU::STX, 4, 2 };
	ret[0x97] = { "SAX", AdressingMode::ZeroPageIndexedY, &CPU::ZeroPageIndexedY, &CPU::SAX, 4, 2 };
	ret[0x98] = { "TYA", AdressingMode::Implied, &CPU::Implied, &CPU::TYA, 2, 1 };
	ret[0x99] = { "STA", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::STA, 5, 3 };
	ret[0x9a] = { "TXS", AdressingMode::Implied, &CPU::Implied, &CPU::TXS, 2, 1 };
	ret[0x9b] = { "TAS", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x9c] = { "SHY", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x9d] = { "STA", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::STA, 5, 3 };
	ret[0x9e] = { "SHX", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0x9f] = { "AHX", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented

	ret[0xa0] = { "LDY", AdressingMode::Immediate, &CPU::Immediate, &CPU::LDY, 2, 2 };
	ret[0xa1] = { "LDA", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::LDA, 6, 2 };
	ret[0xa2] = { "LDX", AdressingMode::Immediate, &CPU::Immediate, &CPU::LDX, 2, 2 };
	ret[0xa3] = { "LAX", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::LAX, 6, 2 };
	ret[0xa4] = { "LDY", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::LDY, 3, 2 };
	ret[0xa5] = { "LDA", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::LDA, 3, 2 };
	ret[0xa6] = { "LDX", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::LDX, 3, 2 };
	ret[0xa7] = { "LAX", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::LAX, 3, 2 };
	ret[0xa8] = { "TAY", AdressingMode::Implied, &CPU::Implied, &CPU::TAY, 2, 1 };
	ret[0xa9] = { "LDA", AdressingMode::Immediate, &CPU::Immediate, &CPU::LDA, 2, 2 };
	ret[0xaa] = { "TAX", AdressingMode::Implied, &CPU::Implied, &CPU::TAX, 2, 1 };
	ret[0xab] = { "LAX", AdressingMode::Immediate, &CPU::Immediate, &CPU::LAX, 2, 2 };
	ret[0xac] = { "LDY", AdressingMode::Absolute, &CPU::Absolute, &CPU::LDY, 4, 3 };
	ret[0xad] = { "LDA", AdressingMode::Absolute, &CPU::Absolute, &CPU::LDA, 4, 3 };
	ret[0xae] = { "LDX", AdressingMode::Absolute, &CPU::Absolute, &CPU::LDX, 4, 3 };
	ret[0xaf] = { "LAX", AdressingMode::Absolute, &CPU::Absolute, &CPU::LAX, 4, 3 };

	ret[0xb0] = { "BCS", AdressingMode::Relative, &CPU::Relative, &CPU::BCS, 2, 2 };
	ret[0xb1] = { "LDA", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::LDA, 5, 2 };
	ret[0xb2] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0xb3] = { "LAX", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::LAX, 5, 2 };
	ret[0xb4] = { "LDY", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::LDY, 4, 2 };
	ret[0xb5] = { "LDA", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::LDA, 4, 2 };
	ret[0xb6] = { "LDX", AdressingMode::ZeroPageIndexedY, &CPU::ZeroPageIndexedY, &CPU::LDX, 4, 2 };
	ret[0xb7] = { "LAX", AdressingMode::ZeroPageIndexedY, &CPU::ZeroPageIndexedY, &CPU::LAX, 4, 2 };
	ret[0xb8] = { "CLV", AdressingMode::Implied, &CPU::Implied, &CPU::CLV, 2, 1 };
	ret[0xb9] = { "LDA", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::LDA, 4, 3 };
	ret[0xba] = { "TSX", AdressingMode::Implied, &CPU::Implied, &CPU::TSX, 2, 1 };
	ret[0xbb] = { "LAS", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0xbc] = { "LDY", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::LDY, 4, 3 };
	ret[0xbd] = { "LDA", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::LDA, 4, 3 };
	ret[0xbe] = { "LDX", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::LDX, 4, 3 };
	ret[0xbf] = { "LAX", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::LAX, 4, 3 };

	ret[0xc0] = { "CPY", AdressingMode::Immediate, &CPU::Immediate, &CPU::CPY, 2, 2 };
	ret[0xc1] = { "CMP", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::CMP, 6, 2 };
	ret[0xc2] = { "NOP", AdressingMode::Immediate, &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	ret[0xc3] = { "DCP", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::DCP, 8, 2 };
	ret[0xc4] = { "CPY", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::CPY, 3, 2 };
	ret[0xc5] = { "CMP", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::CMP, 3, 2 };
	ret[0xc6] = { "DEC", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::DEC, 5, 2 };
	ret[0xc7] = { "DCP", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::DCP, 5, 2 };
	ret[0xc8] = { "INY", AdressingMode::Implied, &CPU::Implied, &CPU::INY, 2, 1 };
	ret[0xc9] = { "CMP", AdressingMode::Immediate, &CPU::Immediate, &CPU::CMP, 2, 2 };
	ret[0xca] = { "DEX", AdressingMode::Implied, &CPU::Implied, &CPU::DEX, 2, 1 };
	ret[0xcb] = { "AXS", AdressingMode::Immediate, &CPU::Immediate, &CPU::AXS, 2, 2 };
	ret[0xcc] = { "CPY", AdressingMode::Absolute, &CPU::Absolute, &CPU::CPY, 4, 3 };
	ret[0xcd] = { "CMP", AdressingMode::Absolute, &CPU::Absolute, &CPU::CMP, 4, 3 };
	ret[0xce] = { "DEC", AdressingMode::Absolute, &CPU::Absolute, &CPU::DEC, 6, 3 };
	ret[0xcf] = { "DCP", AdressingMode::Absolute, &CPU::Absolute, &CPU::DCP, 6, 3 };

	ret[0xd0] = { "BNE", AdressingMode::Relative, &CPU::Relative, &CPU::BNE, 2, 2 };
	ret[0xd1] = { "CMP", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::CMP, 5, 2 };
	ret[0xd2] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0xd3] = { "DCP", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::DCP, 5, 2 };
	ret[0xd4] = { "NOP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	ret[0xd5] = { "CMP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::CMP, 4, 2 };
	ret[0xd6] = { "DEC", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::DEC, 6, 2 };
	ret[0xd7] = { "DCP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::DCP, 6, 2 };
	ret[0xd8] = { "CLD", AdressingMode::Implied, &CPU::Implied, &CPU::CLD, 2, 1 };
	ret[0xd9] = { "CMP", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::CMP, 4, 3 };
	ret[0xda] = { "NOP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 2, 1 };
	ret[0xdb] = { "DCP", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::DCP, 7, 3 };
	ret[0xdc] = { "NOP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	ret[0xdd] = { "CMP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::CMP, 4, 3 };
	ret[0xde] = { "DEC", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::DEC, 7, 3 };
	ret[0xdf] = { "DCP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::DCP, 7, 3 };

	ret[0xe0] = { "CPX", AdressingMode::Immediate, &CPU::Immediate, &CPU::CPX, 2, 2 };
	ret[0xe1] = { "SBC", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::SBC, 6, 2 };
	ret[0xe2] = { "NOP", AdressingMode::Immediate, &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	ret[0xe3] = { "ISC", AdressingMode::IndexedIndirectX, &CPU::IndexedIndirectX, &CPU::ISC, 8, 2 };
	ret[0xe4] = { "CPX", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::CPX, 3, 2 };
	ret[0xe5] = { "SBC", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::SBC, 3, 2 };
	ret[0xe6] = { "INC", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::INC, 5, 2 };
	ret[0xe7] = { "ISC", AdressingMode::ZeroPage, &CPU::ZeroPage, &CPU::ISC, 5, 2 };
	ret[0xe8] = { "INX", AdressingMode::Implied, &CPU::Implied, &CPU::INX, 2, 1 };
	ret[0xe9] = { "SBC", AdressingMode::Immediate, &CPU::Immediate, &CPU::SBC, 2, 2 };
	ret[0xea] = { "NOP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 2, 1 };
	ret[0xeb] = { "SBC", AdressingMode::Immediate, &CPU::Immediate, &CPU::SBC, 2, 2 }; // or is this ADC??
	ret[0xec] = { "CPX", AdressingMode::Absolute, &CPU::Absolute, &CPU::CPX, 4, 3 };
	ret[0xed] = { "SBC", AdressingMode::Absolute, &CPU::Absolute, &CPU::SBC, 4, 3 };
	ret[0xee] = { "INC", AdressingMode::Absolute, &CPU::Absolute, &CPU::INC, 6, 3 };
	ret[0xef] = { "ISC", AdressingMode::Absolute, &CPU::Absolute, &CPU::ISC, 6, 3 };

	ret[0xf0] = { "BEQ", AdressingMode::Relative, &CPU::Relative, &CPU::BEQ, 2, 2 };
	ret[0xf1] = { "SBC", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::SBC, 5, 2 };
	ret[0xf2] = { "STP", AdressingMode::Implied, &CPU::Implied, &CPU::Unimplemented, 1, 1 }; // Not implemented
	ret[0xf3] = { "ISC", AdressingMode::IndirectIndexedY, &CPU::IndirectIndexedY, &CPU::ISC, 8, 2 };
	ret[0xf4] = { "NOP", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	ret[0xf5] = { "SBC", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::SBC, 4, 2 };
	ret[0xf6] = { "INC", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::INC, 6, 2 };
	ret[0xf7] = { "ISC", AdressingMode::ZeroPageIndexedX, &CPU::ZeroPageIndexedX, &CPU::ISC, 6, 2 };
	ret[0xf8] = { "SED", AdressingMode::Implied, &CPU::Implied, &CPU::SED, 4, 1 };
	ret[0xf9] = { "SBC", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::SBC, 4, 3 };
	ret[0xfa] = { "NOP", AdressingMode::Implied, &CPU::Implied, &CPU::NOP, 2, 1 };
	ret[0xfb] = { "ISC", AdressingMode::AbsoluteIndexedY, &CPU::AbsoluteIndexedY, &CPU::ISC, 7, 3 };
	ret[0xfc] = { "NOP", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	ret[0xfd] = { "SBC", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::SBC, 4, 3 };
	ret[0xfe] = { "INC", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::INC, 7, 3 };
	ret[0xff] = { "ISC", AdressingMode::AbsoluteIndexedX, &CPU::AbsoluteIndexedX, &CPU::ISC, 7, 3 };
	// Wow that was a lot

	return ret;
}
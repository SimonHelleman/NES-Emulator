#include "CPU.h"

CPU::CPU(MemoryMap& memory, Disassembler* disassembler)
	: _memory(memory), _opcodeMatrix(std::make_unique<Opcode[]>(256)), _disassembler(disassembler)
{
	// Not implmented:
	// STP: Unofficial opcode that puts the CPU into a low power state until a
	//      hardware interrupt occurs. No games use this apparently.
	// XAA: Unofficial opcode that has unpredictable behaviour which depends on analog effects.
	// AHX: Unofficial opcode that has no documentation
	// TAS: Unofficial opcode that has no documentation
	// SHY: Unofficial opcode that has no documentation
	// SHX: Unofficial opcode that has no documentation
	// LAS: Unofficial opcode that has no documentation
	_opcodeMatrix[0x00] = { "BRK", &CPU::Implied, &CPU::BRK, 7, 1 };
	_opcodeMatrix[0x01] = { "ORA", &CPU::IndexedIndirectX, &CPU::ORA, 6, 2 };
	_opcodeMatrix[0x02] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implmeneted
	_opcodeMatrix[0x03] = { "SLO", &CPU::IndexedIndirectX, &CPU::SLO, 8, 2 };
	_opcodeMatrix[0x04] = { "NOP", &CPU::ZeroPage, &CPU::NOP, 4, 2 };
	_opcodeMatrix[0x05] = { "ORA", &CPU::ZeroPage, &CPU::ORA, 3, 2 };
	_opcodeMatrix[0x06] = { "ASL", &CPU::ZeroPage, &CPU::ASL, 5, 2 };
	_opcodeMatrix[0x07] = { "SLO", &CPU::ZeroPage, &CPU::SLO, 5, 2 };
	_opcodeMatrix[0x08] = { "PHP", &CPU::Implied, &CPU::PHP, 3, 1 };
	_opcodeMatrix[0x09] = { "ORA", &CPU::Immediate, &CPU::ORA, 2, 2 };
	_opcodeMatrix[0x0a] = { "ASL", &CPU::Accumulator, &CPU::ASL, 2, 1 };
	_opcodeMatrix[0x0b] = { "ANC", &CPU::Immediate, &CPU::ANC, 2, 2 };
	_opcodeMatrix[0x0c] = { "NOP", &CPU::Absolute, &CPU::NOP, 4, 3 };
	_opcodeMatrix[0x0d] = { "ORA", &CPU::Absolute, &CPU::ORA, 4, 3 };
	_opcodeMatrix[0x0e] = { "ASL", &CPU::Absolute, &CPU::ASL, 6, 3 };
	_opcodeMatrix[0x0f] = { "SLO", &CPU::Absolute, &CPU::SLO, 6, 3 };

	_opcodeMatrix[0x10] = { "BPL", &CPU::Relative, &CPU::BPL, 2, 2 };
	_opcodeMatrix[0x11] = { "ORA", &CPU::IndirectIndexedY, &CPU::ORA, 5, 2 };
	_opcodeMatrix[0x12] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implmented
	_opcodeMatrix[0x13] = { "SLO", &CPU::IndirectIndexedY, &CPU::SLO, 8, 2 };
	_opcodeMatrix[0x14] = { "NOP", &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	_opcodeMatrix[0x15] = { "ORA", &CPU::ZeroPageIndexedX, &CPU::ORA, 4, 2 };
	_opcodeMatrix[0x16] = { "ASL", &CPU::ZeroPageIndexedX, &CPU::ASL, 6, 2 };
	_opcodeMatrix[0x17] = { "SLO", &CPU::ZeroPageIndexedX, &CPU::SLO, 6, 2 };
	_opcodeMatrix[0x18] = { "CLC", &CPU::Implied, &CPU::CLC, 2, 1 };
	_opcodeMatrix[0x19] = { "ORA", &CPU::AbsoluteIndexedY, &CPU::ORA, 4, 3 };
	_opcodeMatrix[0x1a] = { "NOP", &CPU::Implied, &CPU::NOP, 2, 1 };
	_opcodeMatrix[0x1b] = { "SLO", &CPU::AbsoluteIndexedY, &CPU::SLO, 7, 3 };
	_opcodeMatrix[0x1c] = { "NOP", &CPU::AbsoluteIndexedX, &CPU::NOP, 4, 3 }; // aka IGN
	_opcodeMatrix[0x1d] = { "ORA", &CPU::AbsoluteIndexedX, &CPU::ORA, 4, 3 };
	_opcodeMatrix[0x1e] = { "ASL", &CPU::AbsoluteIndexedX, &CPU::ASL, 7, 3 };
	_opcodeMatrix[0x1f] = { "SLO", &CPU::AbsoluteIndexedX, &CPU::SLO, 7, 3 };

	_opcodeMatrix[0x20] = { "JSR", &CPU::Absolute, &CPU::JSR, 6, 3 };
	_opcodeMatrix[0x21] = { "AND", &CPU::IndexedIndirectX, &CPU::AND, 6, 2 };
	_opcodeMatrix[0x22] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x23] = { "RLA", &CPU::IndexedIndirectX, &CPU::RLA, 8, 2 };
	_opcodeMatrix[0x24] = { "BIT", &CPU::ZeroPage, &CPU::BIT, 3, 2 };
	_opcodeMatrix[0x25] = { "AND", &CPU::ZeroPage, &CPU::AND, 3, 2 };
	_opcodeMatrix[0x26] = { "ROL", &CPU::ZeroPage, &CPU::ROL, 5, 2 };
	_opcodeMatrix[0x27] = { "RLA", &CPU::ZeroPage, &CPU::RLA, 5, 2 };
	_opcodeMatrix[0x28] = { "PLP", &CPU::Implied, &CPU::PLP, 4, 1 };
	_opcodeMatrix[0x29] = { "AND", &CPU::Immediate, &CPU::AND, 2, 2 };
	_opcodeMatrix[0x2a] = { "ROL", &CPU::Accumulator, &CPU::ROL, 2, 1 };
	_opcodeMatrix[0x2b] = { "ANC", &CPU::Immediate, &CPU::ANC, 2, 2 };
	_opcodeMatrix[0x2c] = { "BIT", &CPU::Absolute, &CPU::BIT, 4, 3 };
	_opcodeMatrix[0x2d] = { "AND", &CPU::Absolute, &CPU::AND, 4, 3 };
	_opcodeMatrix[0x2e] = { "ROL", &CPU::Absolute, &CPU::ROL, 6, 3 };
	_opcodeMatrix[0x2f] = { "RLA", &CPU::Absolute, &CPU::RLA, 6, 3 };

	_opcodeMatrix[0x30] = { "BMI", &CPU::Relative, &CPU::BMI, 2, 2 };
	_opcodeMatrix[0x31] = { "AND", &CPU::IndirectIndexedY, &CPU::AND, 5, 2 };
	_opcodeMatrix[0x32] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 };
	_opcodeMatrix[0x33] = { "RLA", &CPU::IndirectIndexedY, &CPU::RLA, 8, 2 };
	_opcodeMatrix[0x34] = { "NOP", &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	_opcodeMatrix[0x35] = { "AND", &CPU::ZeroPageIndexedX, &CPU::AND, 4, 2 };
	_opcodeMatrix[0x36] = { "ROL", &CPU::ZeroPageIndexedX, &CPU::ROL, 5, 2 };
	_opcodeMatrix[0x37] = { "RLA", &CPU::ZeroPageIndexedX, &CPU::RLA, 6, 2 };
	_opcodeMatrix[0x38] = { "SEC", &CPU::Implied, &CPU::SEC, 2, 1 };
	_opcodeMatrix[0x39] = { "AND", &CPU::AbsoluteIndexedY, &CPU::AND, 4, 3 };
	_opcodeMatrix[0x3a] = { "NOP", &CPU::Implied, &CPU::NOP, 2, 1 };
	_opcodeMatrix[0x3b] = { "RLA", &CPU::AbsoluteIndexedY, &CPU::RLA, 7, 3 };
	_opcodeMatrix[0x3c] = { "NOP", &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 };
	_opcodeMatrix[0x3d] = { "AND", &CPU::AbsoluteIndexedX, &CPU::AND, 4, 3 };
	_opcodeMatrix[0x3e] = { "ROL", &CPU::AbsoluteIndexedX, &CPU::ROL, 7, 3 };
	_opcodeMatrix[0x3f] = { "RLA", &CPU::AbsoluteIndexedX, &CPU::RLA, 7, 3 };

	_opcodeMatrix[0x40] = { "RTI", &CPU::Implied, &CPU::RTI, 6, 1 };
	_opcodeMatrix[0x41] = { "EOR", &CPU::IndexedIndirectX, &CPU::EOR, 6, 2 };
	_opcodeMatrix[0x42] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x43] = { "SRE", &CPU::IndexedIndirectX, &CPU::SRE, 8, 2 };
	_opcodeMatrix[0x44] = { "NOP", &CPU::ZeroPage, &CPU::NOP, 3, 2 }; // aka IGN
	_opcodeMatrix[0x45] = { "EOR", &CPU::ZeroPage, &CPU::EOR, 3, 2 };
	_opcodeMatrix[0x46] = { "LSR", &CPU::ZeroPage, &CPU::LSR, 5, 2 };
	_opcodeMatrix[0x47] = { "SRE", &CPU::ZeroPage, &CPU::SRE, 5, 2 };
	_opcodeMatrix[0x48] = { "PHA", &CPU::Implied, &CPU::PHA, 3, 1 };
	_opcodeMatrix[0x49] = { "EOR", &CPU::Immediate, &CPU::EOR, 2, 2 };
	_opcodeMatrix[0x4a] = { "LSR", &CPU::Accumulator, &CPU::LSR, 2, 1 };
	_opcodeMatrix[0x4b] = { "ALR", &CPU::Immediate, &CPU::ALR, 2, 2 };
	_opcodeMatrix[0x4c] = { "JMP", &CPU::Absolute, &CPU::JMP, 3, 3 };
	_opcodeMatrix[0x4d] = { "EOR", &CPU::Absolute, &CPU::EOR, 4, 3 };
	_opcodeMatrix[0x4e] = { "LSR", &CPU::Absolute, &CPU::LSR, 6, 3 };
	_opcodeMatrix[0x4f] = { "SRE", &CPU::Absolute, &CPU::SRE, 6, 3 };

	_opcodeMatrix[0x50] = { "BVC", &CPU::Relative, &CPU::BVC, 2, 2 };
	_opcodeMatrix[0x51] = { "EOR", &CPU::IndirectIndexedY, &CPU::EOR, 5, 2 };
	_opcodeMatrix[0x52] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x53] = { "SRE", &CPU::IndirectIndexedY, &CPU::SRE, 8, 2 };
	_opcodeMatrix[0x54] = { "NOP", &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	_opcodeMatrix[0x55] = { "EOR", &CPU::ZeroPageIndexedX, &CPU::EOR, 4, 2 };
	_opcodeMatrix[0x56] = { "LSR", &CPU::ZeroPageIndexedX, &CPU::LSR, 6, 2 };
	_opcodeMatrix[0x57] = { "SRE", &CPU::ZeroPageIndexedX, &CPU::SRE, 6, 2 };
	_opcodeMatrix[0x58] = { "CLI", &CPU::Implied, &CPU::CLI, 2, 1 };
	_opcodeMatrix[0x59] = { "EOR", &CPU::AbsoluteIndexedY, &CPU::EOR, 4, 3 };
	_opcodeMatrix[0x5a] = { "NOP", &CPU::Implied, &CPU::NOP, 2, 1 };
	_opcodeMatrix[0x5b] = { "SRE", &CPU::AbsoluteIndexedY, &CPU::SRE, 7, 3 };
	_opcodeMatrix[0x5c] = { "NOP", &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	_opcodeMatrix[0x5d] = { "EOR", &CPU::AbsoluteIndexedX, &CPU::EOR, 4, 3 };
	_opcodeMatrix[0x5e] = { "LSR", &CPU::AbsoluteIndexedX, &CPU::LSR, 7, 3 };
	_opcodeMatrix[0x5f] = { "SRE", &CPU::AbsoluteIndexedX, &CPU::SRE, 7, 3 };

	_opcodeMatrix[0x60] = { "RTS", &CPU::Implied, &CPU::RTS, 6, 1 };
	_opcodeMatrix[0x61] = { "ADC", &CPU::IndexedIndirectX, &CPU::ADC, 6, 2 };
	_opcodeMatrix[0x62] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x63] = { "RRA", &CPU::IndexedIndirectX, &CPU::RRA, 8, 2 };
	_opcodeMatrix[0x64] = { "NOP", &CPU::ZeroPage, &CPU::NOP, 3, 2 }; // aka IGN
	_opcodeMatrix[0x65] = { "ADC", &CPU::ZeroPage, &CPU::ADC, 3, 2 };
	_opcodeMatrix[0x66] = { "ROR", &CPU::ZeroPage, &CPU::ROR, 5, 2 };
	_opcodeMatrix[0x67] = { "RRA", &CPU::ZeroPage, &CPU::RRA, 5, 2 };
	_opcodeMatrix[0x68] = { "PLA", &CPU::Implied, &CPU::PLA, 4, 1 };
	_opcodeMatrix[0x69] = { "ADC", &CPU::Immediate, &CPU::ADC, 2, 2 };
	_opcodeMatrix[0x6a] = { "ROR", &CPU::Implied, &CPU::ROR, 2, 1 };
	_opcodeMatrix[0x6b] = { "ARR", &CPU::Immediate, &CPU::ARR, 2, 2 };
	_opcodeMatrix[0x6c] = { "JMP", &CPU::Indirect, &CPU::JMP, 5, 3 };
	_opcodeMatrix[0x6d] = { "ADC", &CPU::Absolute, &CPU::ADC, 4, 3 };
	_opcodeMatrix[0x6e] = { "ROR", &CPU::Absolute, &CPU::ROR, 6, 3 };
	_opcodeMatrix[0x6f] = { "RRA", &CPU::Absolute, &CPU::RRA, 6, 3 };

	_opcodeMatrix[0x70] = { "BVS", &CPU::Relative, &CPU::BVS, 2, 2 };
	_opcodeMatrix[0x71] = { "ADC", &CPU::IndirectIndexedY, &CPU::ADC, 5, 2 };
	_opcodeMatrix[0x72] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x73] = { "RRA", &CPU::IndirectIndexedY, &CPU::RRA, 8, 2 };
	_opcodeMatrix[0x74] = { "NOP", &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	_opcodeMatrix[0x75] = { "ADC", &CPU::ZeroPageIndexedX, &CPU::ADC, 4, 2 };
	_opcodeMatrix[0x76] = { "ROR", &CPU::ZeroPageIndexedX, &CPU::ROR, 6, 2 };
	_opcodeMatrix[0x77] = { "RRA", &CPU::ZeroPageIndexedX, &CPU::RRA, 6, 2 };
	_opcodeMatrix[0x78] = { "SEI", &CPU::Implied, &CPU::SEI, 2, 1 };
	_opcodeMatrix[0x79] = { "ADC", &CPU::AbsoluteIndexedY, &CPU::ADC, 4, 3 };
	_opcodeMatrix[0x7a] = { "NOP", &CPU::Implied, &CPU::NOP, 2, 1 };
	_opcodeMatrix[0x7b] = { "RRA", &CPU::AbsoluteIndexedY, &CPU::RRA, 7, 3 };
	_opcodeMatrix[0x7c] = { "NOP", &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	_opcodeMatrix[0x7d] = { "ADC", &CPU::AbsoluteIndexedX, &CPU::ADC, 4, 3 };
	_opcodeMatrix[0x7e] = { "ROR", &CPU::AbsoluteIndexedX, &CPU::ROR, 7, 3 };
	_opcodeMatrix[0x7f] = { "RRA", &CPU::AbsoluteIndexedX, &CPU::RRA, 7, 3 };
	
	_opcodeMatrix[0x80] = { "NOP", &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SBK
	_opcodeMatrix[0x81] = { "STA", &CPU::IndexedIndirectX, &CPU::STA, 6, 2 };
	_opcodeMatrix[0x82] = { "NOP", &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	_opcodeMatrix[0x83] = { "SAX", &CPU::IndexedIndirectX, &CPU::SAX, 6, 2 };
	_opcodeMatrix[0x84] = { "STY", &CPU::ZeroPage, &CPU::STY, 3, 2 };
	_opcodeMatrix[0x85] = { "STA", &CPU::ZeroPage, &CPU::STA, 3, 2 };
	_opcodeMatrix[0x86] = { "STX", &CPU::ZeroPage, &CPU::STX, 3, 2 };
	_opcodeMatrix[0x87] = { "SAX", &CPU::ZeroPage, &CPU::SAX, 3, 2 };
	_opcodeMatrix[0x88] = { "DEY", &CPU::Implied, &CPU::DEY, 2, 1 };
	_opcodeMatrix[0x89] = { "NOP", &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	_opcodeMatrix[0x8a] = { "TXA", &CPU::Implied, &CPU::TXA, 2, 1 };
	_opcodeMatrix[0x8b] = { "XAA", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x8c] = { "STY", &CPU::Absolute, &CPU::STY, 4, 3 };
	_opcodeMatrix[0x8d] = { "STA", &CPU::Absolute, &CPU::STA, 4, 3 };
	_opcodeMatrix[0x8e] = { "STX", &CPU::Absolute, &CPU::STX, 4, 3 };
	_opcodeMatrix[0x8f] = { "SAX", &CPU::Absolute, &CPU::SAX, 4, 3 };

	_opcodeMatrix[0x90] = { "BCC", &CPU::Relative, &CPU::BCC, 2, 2 };
	_opcodeMatrix[0x91] = { "STA", &CPU::IndirectIndexedY, &CPU::STA, 6, 2 };
	_opcodeMatrix[0x92] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x93] = { "AHX", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x94] = { "STY", &CPU::ZeroPageIndexedX, &CPU::STY, 4, 2 };
	_opcodeMatrix[0x95] = { "STA", &CPU::ZeroPageIndexedX, &CPU::STA, 4, 2 };
	_opcodeMatrix[0x96] = { "STA", &CPU::ZeroPageIndexedY, &CPU::STX, 4, 2 };
	_opcodeMatrix[0x97] = { "SAX", &CPU::ZeroPageIndexedY, &CPU::SAX, 4, 2 };
	_opcodeMatrix[0x98] = { "TYA", &CPU::Implied, &CPU::TYA, 2, 1 };
	_opcodeMatrix[0x99] = { "STA", &CPU::AbsoluteIndexedY, &CPU::STA, 5, 3 };
	_opcodeMatrix[0x9a] = { "TXS", &CPU::Implied, &CPU::TXS, 2, 1 };
	_opcodeMatrix[0x9b] = { "TAS", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x9c] = { "SHY", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x9d] = { "STA", &CPU::AbsoluteIndexedX, &CPU::STA, 5, 3 };
	_opcodeMatrix[0x9e] = { "SHX", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0x9f] = { "AHX", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented

	_opcodeMatrix[0xa0] = { "LDY", &CPU::Immediate, &CPU::LDY, 2, 2 };
	_opcodeMatrix[0xa1] = { "LDA", &CPU::IndexedIndirectX, &CPU::LDA, 6, 2 };
	_opcodeMatrix[0xa2] = { "LDX", &CPU::Immediate, &CPU::LDX, 2, 2 };
	_opcodeMatrix[0xa3] = { "LAX", &CPU::IndexedIndirectX, &CPU::LAX, 6, 2 };
	_opcodeMatrix[0xa4] = { "LDY", &CPU::ZeroPage, &CPU::LDY, 3, 2 };
	_opcodeMatrix[0xa5] = { "LDA", &CPU::ZeroPage, &CPU::LDA, 3, 2 };
	_opcodeMatrix[0xa6] = { "LDX", &CPU::ZeroPage, &CPU::LDX, 3, 2 };
	_opcodeMatrix[0xa7] = { "LAX", &CPU::ZeroPage, &CPU::LAX, 3, 2 };
	_opcodeMatrix[0xa8] = { "TAY", &CPU::Implied, &CPU::TAY, 2, 1 };
	_opcodeMatrix[0xa9] = { "LDA", &CPU::Immediate, &CPU::LDA, 2, 2 };
	_opcodeMatrix[0xaa] = { "TAX", &CPU::Implied, &CPU::TAX, 2, 1 };
	_opcodeMatrix[0xab] = { "LAX", &CPU::Immediate, &CPU::LAX, 2, 2 };
	_opcodeMatrix[0xac] = { "LDY", &CPU::Absolute, &CPU::LDY, 4, 3 };
	_opcodeMatrix[0xad] = { "LDA", &CPU::Absolute, &CPU::LDA, 4, 3 };
	_opcodeMatrix[0xae] = { "LDX", &CPU::Absolute, &CPU::LDX, 4, 3 };
	_opcodeMatrix[0xaf] = { "LAX", &CPU::Absolute, &CPU::LAX, 4, 3 };

	_opcodeMatrix[0xb0] = { "BCS", &CPU::Relative, &CPU::BCS, 2, 2 };
	_opcodeMatrix[0xb1] = { "LDA", &CPU::IndirectIndexedY, &CPU::LDA, 5, 2 };
	_opcodeMatrix[0xb2] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0xb3] = { "LAX", &CPU::IndirectIndexedY, &CPU::LAX, 5, 2 };
	_opcodeMatrix[0xb4] = { "LDY", &CPU::ZeroPageIndexedX, &CPU::LDY, 4, 2};
	_opcodeMatrix[0xb5] = { "LDA", &CPU::ZeroPageIndexedX, &CPU::LDA, 4, 2 };
	_opcodeMatrix[0xb6] = { "LDX", &CPU::ZeroPageIndexedY, &CPU::LDX, 4, 2 };
	_opcodeMatrix[0xb7] = { "LAX", &CPU::ZeroPageIndexedY, &CPU::LAX, 4, 2 };
	_opcodeMatrix[0xb8] = { "CLV", &CPU::Implied, &CPU::CLV, 2, 1 };
	_opcodeMatrix[0xb9] = { "LDA", &CPU::AbsoluteIndexedY, &CPU::LDA, 4, 3 };
	_opcodeMatrix[0xba] = { "TSX", &CPU::Implied, &CPU::TSX, 2, 1 };
	_opcodeMatrix[0xbb] = { "LAS", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0xbc] = { "LDY", &CPU::AbsoluteIndexedX, &CPU::LDY, 4, 3 };
	_opcodeMatrix[0xbd] = { "LDA", &CPU::AbsoluteIndexedX, &CPU::LDA, 4, 3 };
	_opcodeMatrix[0xbe] = { "LDX", &CPU::AbsoluteIndexedY, &CPU::LDX, 4, 3 };
	_opcodeMatrix[0xbf] = { "LAX", &CPU::AbsoluteIndexedY, &CPU::LAX, 4, 3 };

	_opcodeMatrix[0xc0] = { "CPY", &CPU::Immediate, &CPU::CPY, 2, 2 };
	_opcodeMatrix[0xc1] = { "CMP", &CPU::IndexedIndirectX, &CPU::CMP, 6, 2 };
	_opcodeMatrix[0xc2] = { "NOP", &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	_opcodeMatrix[0xc3] = { "DCP", &CPU::IndexedIndirectX, &CPU::DCP, 8, 2 };
	_opcodeMatrix[0xc4] = { "CPY", &CPU::ZeroPage, &CPU::CPY, 3, 2 };
	_opcodeMatrix[0xc5] = { "CMP", &CPU::ZeroPage, &CPU::CMP, 3, 2 };
	_opcodeMatrix[0xc6] = { "DEC", &CPU::ZeroPage, &CPU::DEC, 5, 2 };
	_opcodeMatrix[0xc7] = { "DCP", &CPU::ZeroPage, &CPU::DCP, 5, 2 };
	_opcodeMatrix[0xc8] = { "INY", &CPU::Implied, &CPU::INY, 2, 1 };
	_opcodeMatrix[0xc9] = { "CMP", &CPU::Immediate, &CPU::CMP, 2, 2 };
	_opcodeMatrix[0xca] = { "DEX", &CPU::Implied, &CPU::DEX, 2, 1 };
	_opcodeMatrix[0xcb] = { "AXS", &CPU::Immediate, &CPU::AXS, 2, 2 };
	_opcodeMatrix[0xcc] = { "CPY", &CPU::Absolute, &CPU::CPY, 4, 3 };
	_opcodeMatrix[0xcd] = { "CMP", &CPU::Absolute, &CPU::CMP, 4, 3 };
	_opcodeMatrix[0xce] = { "DEC", &CPU::Absolute, &CPU::DEC, 6, 3 };
	_opcodeMatrix[0xcf] = { "DCP", &CPU::Absolute, &CPU::DCP, 6, 3 };

	_opcodeMatrix[0xd0] = { "BNE", &CPU::Relative, &CPU::BNE, 2, 2 };
	_opcodeMatrix[0xd1] = { "CMP", &CPU::IndirectIndexedY, &CPU::CMP, 5, 2 };
	_opcodeMatrix[0xd2] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented
	_opcodeMatrix[0xd3] = { "DCP", &CPU::IndirectIndexedY, &CPU::DCP, 5, 2 };
	_opcodeMatrix[0xd4] = { "NOP", &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	_opcodeMatrix[0xd5] = { "CMP", &CPU::ZeroPageIndexedX, &CPU::CMP, 4, 2 };
	_opcodeMatrix[0xd6] = { "DEC", &CPU::ZeroPageIndexedX, &CPU::DEC, 6, 2 };
	_opcodeMatrix[0xd7] = { "DCP", &CPU::ZeroPageIndexedX, &CPU::DCP, 6, 2 };
	_opcodeMatrix[0xd8] = { "CLD", &CPU::Implied, &CPU::CLD, 2, 1 };
	_opcodeMatrix[0xd9] = { "CMP", &CPU::AbsoluteIndexedY, &CPU::CMP, 4, 3 };
	_opcodeMatrix[0xda] = { "NOP", &CPU::Implied, &CPU::NOP, 2, 1 };
	_opcodeMatrix[0xdb] = { "DCP", &CPU::AbsoluteIndexedY, &CPU::DCP, 7, 3 };
	_opcodeMatrix[0xdc] = { "NOP", &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	_opcodeMatrix[0xdd] = { "CMP", &CPU::AbsoluteIndexedX, &CPU::CMP, 4, 3 };
	_opcodeMatrix[0xde] = { "DEC", &CPU::AbsoluteIndexedX, &CPU::DEC, 7, 3 };
	_opcodeMatrix[0xdf] = { "DCP", &CPU::AbsoluteIndexedX, &CPU::DCP, 7, 3 };

	_opcodeMatrix[0xe0] = { "CPX", &CPU::Immediate, &CPU::CPX, 2, 2 };
	_opcodeMatrix[0xe1] = { "SBC", &CPU::IndexedIndirectX, &CPU::SBC, 6, 2 };
	_opcodeMatrix[0xe2] = { "NOP", &CPU::Immediate, &CPU::NOP, 2, 2 }; // aka SKB
	_opcodeMatrix[0xe3] = { "ISC", &CPU::IndexedIndirectX, &CPU::ISC, 8, 2 };
	_opcodeMatrix[0xe4] = { "CPX", &CPU::ZeroPage, &CPU::CPX, 3, 2 };
	_opcodeMatrix[0xe5] = { "SBC", &CPU::ZeroPage, &CPU::SBC, 3, 2 };
	_opcodeMatrix[0xe6] = { "INC", &CPU::ZeroPage, &CPU::INC, 5, 2 };
	_opcodeMatrix[0xe7] = { "ISC", &CPU::ZeroPage, &CPU::ISC, 5, 2 };
	_opcodeMatrix[0xe8] = { "INX", &CPU::Implied, &CPU::INX, 2, 1 };
	_opcodeMatrix[0xe9] = { "SBC", &CPU::Immediate, &CPU::SBC, 2, 2 };
	_opcodeMatrix[0xea] = { "NOP", &CPU::Implied, &CPU::NOP, 2, 1 };
	_opcodeMatrix[0xeb] = { "SBC", &CPU::Immediate, &CPU::SBC, 2, 2 }; // or is this ADC??
	_opcodeMatrix[0xec] = { "CPX", &CPU::Absolute, &CPU::CPX, 4, 3 };
	_opcodeMatrix[0xed] = { "SBC", &CPU::Absolute, &CPU::SBC, 4, 3 };
	_opcodeMatrix[0xee] = { "INC", &CPU::Absolute, &CPU::INC, 6, 3 };
	_opcodeMatrix[0xef] = { "ISC", &CPU::Absolute, &CPU::ISC, 6, 3 };

	_opcodeMatrix[0xf0] = { "BEQ", &CPU::Relative, &CPU::BEQ, 2, 2 };
	_opcodeMatrix[0xf1] = { "SBC", &CPU::IndirectIndexedY, &CPU::SBC, 5, 2 };
	_opcodeMatrix[0xf2] = { "STP", &CPU::Implied, &CPU::NOP, -1, 1 }; // Not implemented 
	_opcodeMatrix[0xf3] = { "ISC", &CPU::IndirectIndexedY, &CPU::ISC, 8, 2 };
	_opcodeMatrix[0xf4] = { "NOP", &CPU::ZeroPageIndexedX, &CPU::NOP, 4, 2 }; // aka IGN
	_opcodeMatrix[0xf5] = { "SBC", &CPU::ZeroPageIndexedX, &CPU::SBC, 4, 2 };
	_opcodeMatrix[0xf6] = { "INC", &CPU::ZeroPageIndexedX, &CPU::INC, 6, 2 };
	_opcodeMatrix[0xf7] = { "ISC", &CPU::ZeroPageIndexedX, &CPU::ISC, 6, 2 };
	_opcodeMatrix[0xf8] = { "SED", &CPU::Implied, &CPU::SED, 4, 1 };
	_opcodeMatrix[0xf9] = { "SBC", &CPU::AbsoluteIndexedY, &CPU::SBC, 4, 3 };
	_opcodeMatrix[0xfa] = { "NOP", &CPU::Implied, &CPU::NOP, 2, 1 };
	_opcodeMatrix[0xfb] = { "ISC", &CPU::AbsoluteIndexedY, &CPU::ISC, 7, 3 };
	_opcodeMatrix[0xfc] = { "NOP", &CPU::AbsoluteIndexedX, &CPU::NOP, 5, 3 }; // aka IGN
	_opcodeMatrix[0xfd] = { "SBC", &CPU::AbsoluteIndexedX, &CPU::SBC, 4, 3 };
	_opcodeMatrix[0xfe] = { "INC", &CPU::AbsoluteIndexedX, &CPU::INC, 7, 3 };
	_opcodeMatrix[0xff] = { "ISC", &CPU::AbsoluteIndexedX, &CPU::ISC, 7, 3 };
	// Wow that was a lot
}

void CPU::Clock()
{
	if (_currentState == State::Fetch)
	{
		if (_doNMI)
		{
			_isIRQPending = _doIRQ && STATUS_I;
			HandleNMI();
		}
		else if ((_doIRQ && STATUS_I) || _isIRQPending)
		{
			BRK();
			_isIRQPending = false;
		}

		FetchInstruction();
		_currentState = State::AddressingMode;
		return;
	}

	if (_currentState == State::AddressingMode)
	{
		_currentInstruction.addrMode(this);
		_currentState = State::Execute;
	}

	if (_currentState == State::Execute)
	{
		if (_currentInstruction.clockCycles > 0)
		{
			--_currentInstruction.clockCycles;
			return;
		}

		_currentInstruction.operation(this);
		_currentState = State::Fetch;
	}
}

void CPU::FetchInstruction()
{
	uint8_t opcode = _memory.Read(_regPC);

	// The copy is intentional since operations
	// that need more than one clock cycle will modify
	// the cycle count
	_currentInstruction = _opcodeMatrix[opcode];

	if (_doNMI || (_doIRQ && STATUS_I))
	{
		_currentInstruction.clockCycles += 2;
	}

	if (_disassembler != nullptr)
	{
		_disassembler->AddInstruction(_regPC++, _currentInstruction.mnemonic, _currentInstruction.size);
	}
}

void CPU::Reset()
{
	_regStatus = 0;
	_regStatus = STATUS_5 | STATUS_B | STATUS_I;
	_regA = 0;
	_regX = 0;
	_regY = 0;
	_regSP = 0xfd;
	_regPC = RESET_VECTOR;
	_currentInstruction = _opcodeMatrix[0x4c];
	_currentState = State::AddressingMode;
}

void CPU::HandleNMI()
{
	_memory.Write(0x0100 | _regSP--, (--_regPC) >> 8);
	_memory.Write(0x0100 | _regSP--, _regPC & 0b11111111);


	_regPC = NMI_VECTOR;

	_doNMI = false;
}

void CPU::BRK()
{
	_memory.Write(0x0100 | _regSP--, (--_regPC) >> 8);
	_memory.Write(0x0100 | _regSP--, _regPC & 0b11111111);

	_regStatus |= STATUS_B;
	_regPC = IRQ_VECTOR;
}

void CPU::JSR()
{
	// Push return address to stack
	_memory.Write(0x0100 | _regSP--, (--_regPC) >> 8);
	_memory.Write(0x0100 | _regSP--, _regPC & 0b11111111);
	_memory.Write(0x0100 | _regSP--, _regStatus);

	_regPC = _currentAddr;
}

void CPU::RTI()
{
	_regStatus = _memory.Read(0x100 | ++_regSP);
	uint8_t retAddrLow = _memory.Read(0x0100 | ++_regSP);
	uint8_t retAddrHigh = _memory.Read(0x0100 | ++_regSP);

	_regPC = (retAddrHigh << 8 | retAddrLow) + 1;

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
	_regA += static_cast<uint8_t>(addend) + (_regStatus & STATUS_C) ? 1 : 0;

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
	_regA -= static_cast<uint8_t>(subtrahend) + (_regStatus & STATUS_C) ? 0 : 1;

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
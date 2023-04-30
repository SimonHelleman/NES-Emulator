#pragma once
#include <cstdint>
#include <vector>

#include "MemoryMap.h"

class Disassembler
{
public:

	static struct Instruction
	{
		uint16_t address;
		const char* mnemonic;
		int numOperands;
		uint8_t operands[2];
	};

	Disassembler(const MemoryMap& memory)
		: _memory(memory)
	{
	}

	void AddInstruction(uint16_t addr, const char* mnemonic, int size);

private:
	std::vector<Instruction> _disassembly;
	const MemoryMap& _memory;
};


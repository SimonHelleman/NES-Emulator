#pragma once
#include <cstdint>
#include <unordered_map>

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
	std::unordered_map<uint16_t, Instruction> _disassembly;
	const MemoryMap& _memory;
};


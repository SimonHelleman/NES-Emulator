#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

#include "MemoryMap.h"
#include "CPUAdressingMode.h"

class Disassembler
{
public:

	struct Instruction
	{
		uint16_t address;
		const char* mnemonic;
		AdressingMode addrMode;
		int numOperands;
		uint8_t operands[2];
	};

	Disassembler(const MemoryMap& memory)
		: _memory(memory)
	{
	}

	void AddInstruction(uint16_t addr, const char* mnemonic, int size, AdressingMode addrMode);


	const std::unordered_map<uint16_t, Instruction>& GetDisassembly() const
	{
		return _disassembly;
	}

	const Instruction& GetInstruction(uint16_t addr) const
	{
		return _disassembly.at(addr);
	}

	const Instruction& GetLatestInstruction() const
	{
		return _latestInst;
	}

	std::string GetLatestDissamblyLine() const
	{
		return GetDisassemblyLine(_latestInst);
	}

	static std::string GetDisassemblyLine(const Instruction& inst);

private:
	std::unordered_map<uint16_t, Instruction> _disassembly;
	Instruction _latestInst = { 0 };
	const MemoryMap& _memory;
};


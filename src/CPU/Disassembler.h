#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "Memory/MemoryMap.h"
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
		: Disassembler(memory, DEFUALT_CACHE_CAP)
	{
	}

	Disassembler(const MemoryMap& memory, size_t instructionCacheSize)
		: _memory(memory), _instructionCacheCapacity(instructionCacheSize)
	{
		_instructionCache.reserve(_instructionCacheCapacity);
	}

	void AddInstruction(uint16_t addr, const char* mnemonic, int size, AdressingMode addrMode);


	const std::map<uint16_t, Instruction>& GetDisassembly() const
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

	std::vector<Instruction> InstrctionCache() const 
	{
		return _instructionCache;
	}

	void Clear()
	{
		_instructionCache.clear();
		_disassembly.clear();
	}

	std::string GetLatestDissamblyLine() const
	{
		return MakeDisassemblyLine(_latestInst);
	}

	void WriteToFile(const char* filepath) const;

	static std::string MakeDisassemblyLine(const Instruction& inst);

private:
	std::map<uint16_t, Instruction> _disassembly;
	Instruction _latestInst = { 0 };
	const MemoryMap& _memory;

	std::vector<Instruction> _instructionCache;
	size_t _instructionCacheCapacity;
	static constexpr size_t DEFUALT_CACHE_CAP = 10;
};


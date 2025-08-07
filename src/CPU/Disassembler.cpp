#include <cstdio>
#include <fstream>
#include "Disassembler.h"

void Disassembler::AddInstruction(uint16_t addr, const char* mnemonic, int size, AdressingMode addrMode)
{
	Instruction inst = { addr, mnemonic, addrMode, size - 1, { 0, 0 } };

	if (size == 2)
	{
		inst.operands[0] = _memory.Read(addr + 1);
	}

	if (size == 3)
	{
		inst.operands[0] = _memory.Read(addr + 1);
		inst.operands[1] = _memory.Read(addr + 2);
	}

	_disassembly[addr] = inst;
	_latestInst = inst;

	if (_instructionCache.size() < _instructionCacheCapacity)
	{
		_instructionCache.emplace_back(inst);
	}
	else
	{
		for (size_t i = 0; i < _instructionCacheCapacity - 1; ++i)
		{
			_instructionCache[i] = _instructionCache[i + 1];
		}
		_instructionCache[_instructionCacheCapacity - 1] = inst;
	}
}

void Disassembler::WriteToFile(const char* filepath) const
{
	std::ofstream file(filepath);

	for (const auto& i : _disassembly)
	{
		char addrStr[8];

		snprintf(addrStr, 8, "%04x", i.first);
		file << addrStr << "    " << MakeDisassemblyLine(i.second) << '\n';
	}
}

std::string Disassembler::MakeDisassemblyLine(const Instruction& inst)
{
	constexpr size_t BUF_SIZE = 64;
	char buf[BUF_SIZE];

	snprintf(buf, BUF_SIZE, "%s ", inst.mnemonic);
	if (inst.numOperands == 0)
	{
		return std::string(buf);
	}

	std::string ret = buf;

	if (inst.numOperands == 1)
	{
		if (inst.addrMode == AdressingMode::Immediate)
		{
			snprintf(buf, BUF_SIZE, "#%d", inst.operands[0]);
		}
		else if (inst.addrMode == AdressingMode::ZeroPageIndexedX)
		{
			snprintf(buf, BUF_SIZE, "$%02x, x", inst.operands[0]);
		}
		else if (inst.addrMode == AdressingMode::ZeroPageIndexedY)
		{
			snprintf(buf, BUF_SIZE, "$%02x, y", inst.operands[0]);
		}
		else if (inst.addrMode == AdressingMode::IndexedIndirectX)
		{
			snprintf(buf, BUF_SIZE, "($%02x, x)", inst.operands[0]);
		}
		else if (inst.addrMode == AdressingMode::IndirectIndexedY)
		{
			snprintf(buf, BUF_SIZE, "($%02x), y", inst.operands[0]);
		}
		else
		{
			snprintf(buf, BUF_SIZE, "$%02x", inst.operands[0]);
		}

		ret += buf;
		return ret;
	}

	uint16_t absAddr = 0;
	absAddr = (inst.operands[1] << 8) | inst.operands[0];

	if (inst.addrMode == AdressingMode::Absolute)
	{
		snprintf(buf, BUF_SIZE, "$%04x", absAddr);
	}
	else if (inst.addrMode == AdressingMode::AbsoluteIndexedX)
	{
		snprintf(buf, BUF_SIZE, "$%04x, x", absAddr);
	}
	else if (inst.addrMode == AdressingMode::AbsoluteIndexedY)
	{
		snprintf(buf, BUF_SIZE, "$%04x, y", absAddr);
	}
	else if (inst.addrMode == AdressingMode::Indirect)
	{
		snprintf(buf, BUF_SIZE, "($%04x)", absAddr);
	}

	ret += buf;
	return ret;
}
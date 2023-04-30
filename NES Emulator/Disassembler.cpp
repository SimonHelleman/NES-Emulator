#include <iostream>
#include <cstdio>
#include "Disassembler.h"

void Disassembler::AddInstruction(uint16_t addr, const char* mnemonic, int size)
{
	Instruction inst = { addr, mnemonic, size - 1, { 0, 0 } };

	printf("0x%04x    %s ", addr, mnemonic);

	if (size == 1)
	{
		printf("\n");
	}
	
	if (size == 2)
	{
		inst.operands[0] = _memory.Read(addr + 1);
		printf("$%2x\n", inst.operands[0]);
	}

	if (size == 3)
	{
		inst.operands[0] = _memory.Read(addr + 1);
		inst.operands[1] = _memory.Read(addr + 2);

		uint16_t addr = 0;
		addr = (inst.operands[1] << 8) | inst.operands[0];
		printf("$%4x\n", addr);
	}

	_disassembly.push_back(inst);
}

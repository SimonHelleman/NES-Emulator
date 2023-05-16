#include <iostream>
#include <cstdio>
#include "Disassembler.h"

void Disassembler::AddInstruction(uint16_t addr, const char* mnemonic, int size, AdressingMode addrMode)
{
	Instruction inst = { addr, mnemonic, addrMode, size - 1, { 0, 0 } };

	/*
	printf("0x%04x    %s ", addr, mnemonic);

	// This code is kinda grose, just a temporary dump until
	// a GUI is up and running

	if (size == 1)
	{
		printf("\n");
	}
	
	if (size == 2)
	{
		inst.operands[0] = _memory.Read(addr + 1);

		if (addrMode == AdressingMode::Immediate)
		{
			printf("#%d\n", inst.operands[0]);
		}
		else if (addrMode == AdressingMode::ZeroPageIndexedX)
		{
			printf("$%02x, x\n", inst.operands[0]);
		}
		else if (addrMode == AdressingMode::ZeroPageIndexedY)
		{
			printf("$%02x, y\n", inst.operands[0]);
		}
		else if (addrMode == AdressingMode::IndexedIndirectX)
		{
			printf("($%02x, x)\n", inst.operands[0]);
		}
		else if (addrMode == AdressingMode::IndirectIndexedY)
		{
			printf("($%02x), y\n", inst.operands[0]);
		}
		else
		{
			printf("$%02x\n", inst.operands[0]);
		}
	}

	if (size == 3)
	{
		inst.operands[0] = _memory.Read(addr + 1);
		inst.operands[1] = _memory.Read(addr + 2);

		uint16_t absAddr = 0;
		absAddr = (inst.operands[1] << 8) | inst.operands[0];
		
		
		if (addrMode == AdressingMode::Absolute)
		{
			printf("$%04x\n", absAddr);
		}
		else if (addrMode == AdressingMode::AbsoluteIndexedX)
		{
			printf("$%04x, x\n", absAddr);
		}
		else if (addrMode == AdressingMode::AbsoluteIndexedY)
		{
			printf("$%04x, y\n", absAddr);
		}
		else if (addrMode == AdressingMode::Indirect)
		{
			printf("($%04x)\n", absAddr);
		}
	}

	*/

	_disassembly[addr] = inst;
}

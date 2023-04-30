#include <iostream>
#include "RAMOnlyMemMap.h"
#include "CPU.h"

int main()
{	
	std::cout << "hello world\n";

	RAMOnlyMemMap memory;

	for (int i = 0; i < 65536; ++i)
	{
		memory.Write(i, 0xea); // Fill with noop instructions
	}

	memory.Write(0xfffc, 0x01);
	memory.Write(0xfffd, 0x00);

	memory.Write(1, 0xa9);
	memory.Write(2, 0xab); // 171
	memory.Write(3, 0xa0);
	memory.Write(4, 123);
	memory.Write(5, 0xae);
	memory.Write(6, 0xd2);
	memory.Write(7, 0xba);
	memory.Write(0xbad2, 251);
	memory.Write(8, 0x8d);
	memory.Write(9, 0xad);
	memory.Write(10, 0xde);

	Disassembler disassembler = Disassembler(memory);
	CPU cpu = CPU(memory, &disassembler);
	cpu.Reset();

	while (1)
	{
		cpu.Clock();
	}

	return 0;
}
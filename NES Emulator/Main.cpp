#include <iostream>
#include <fstream>
#include "RAMOnlyMemMap.h"
#include "CPU.h"

int main()
{	
	std::cout << "hello world\n";

	RAMOnlyMemMap memory;

	memory.LoadFromFile("eater_subroutine_test.bin");

	std::ofstream memDumpFile("hexdump.txt");
	memDumpFile << memory.HexDump() << '\n';

	memDumpFile.close();
			
	Disassembler disassembler = Disassembler(memory);
	CPU cpu = CPU(memory, &disassembler);
	cpu.Reset();

	while (1)
	{
		cpu.Clock();

		// memDumpFile.open("hexdump.txt");
		// memDumpFile << memory.HexDump();
		// memDumpFile.close();
	}

	return 0;
}
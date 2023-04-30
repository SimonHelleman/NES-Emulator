#include <cstdio>
#include <iostream>
#include <fstream>
#include "RAMOnlyMemMap.h"

void RAMOnlyMemMap::LoadFromFile(const char* filePath)
{
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if (!file.is_open())
	{
		std::cerr << "Error opening file: " << filePath << '\n';
		return;
	}

	std::streamsize fileSize = file.tellg();

	file.seekg(0, std::ios::beg);

	fileSize = fileSize < RAM_SIZE ? fileSize : RAM_SIZE;

	if (!file.read(reinterpret_cast<char*>(_ram.get()), fileSize))
	{
		std::cerr << "Error reading file: " << filePath << '\n';
		return;
	}

	file.close();
}

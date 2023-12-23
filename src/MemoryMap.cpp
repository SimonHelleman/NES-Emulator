#include <sstream>
#include "MemoryMap.h"

std::string MemoryMap::HexDump()
{
	char buf[64];

	std::stringstream ss;

	for (int i = 0; i < ADRESSABLE_RANGE; ++i)
	{
		if (i % 16 == 0)
		{
			if (i > 0)
			{
				ss << '\n';
			}
			snprintf(buf, 64, "%04x: ", i);
			ss << buf;
		}

		snprintf(buf, 64, "%02x ", Read(i));
		ss << buf;
	}

	return ss.str();
}

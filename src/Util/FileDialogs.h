#pragma once
#include <string>

class FileDialogs
{
public:
	static bool Available();
	static std::string OpenFileDialog(const char* filter);
	static std::string SaveFileDialog(const char* filter);
};


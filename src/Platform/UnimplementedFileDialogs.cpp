#ifndef _WIN64
#include "Util/FileDialogs.h"

bool FileDialogs::Available()
{
	return false;
}

std::string FileDialogs::OpenFileDialog(const char* filter)
{
	return "";
}

std::string FileDialogs::SaveFileDialog(const char* filter)
{
	return "";
}

#endif
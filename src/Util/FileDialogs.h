#pragma once
#include <string>
#include <GLFW/glfw3.h>

class FileDialogs
{
public:
	static bool Available();
	static std::string OpenFileDialog(GLFWwindow* windowHandle, const char* filter);
	static std::string SaveFileDialog(GLFWwindow* windowHandle, const char* filter);
};


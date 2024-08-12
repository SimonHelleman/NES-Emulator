#ifdef _WIN64
#include <windows.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "Application.h"
#include "Util/FileDialogs.h"

bool FileDialogs::Available()
{
	return true;
}

std::string FileDialogs::OpenFileDialog(const char* filter)
{
	OPENFILENAMEA dialog = { 0 };
	char path[MAX_PATH] = { 0 };
	char dir[MAX_PATH] = { 0 };
	dialog.lStructSize = sizeof(OPENFILENAME);

	Application& app = Application::Get();
	dialog.hwndOwner = glfwGetWin32Window(app._window);
	dialog.lpstrFile = path;
	dialog.nMaxFile = sizeof(path);

	// Get CWD as start directory
	if (GetCurrentDirectoryA(MAX_PATH, dir))
	{
		dialog.lpstrInitialDir = dir;
	}

	dialog.lpstrFilter = filter;
	dialog.nFilterIndex = 1;

	dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&dialog))
	{
		return dialog.lpstrFile;
	}

	return "";
}

std::string FileDialogs::SaveFileDialog(const char* filter)
{
	OPENFILENAMEA dialog = { 0 };
	char path[MAX_PATH] = { 0 };
	char dir[MAX_PATH] = { 0 };
	dialog.lStructSize = sizeof(OPENFILENAME);

	Application& app = Application::Get();
	dialog.hwndOwner = glfwGetWin32Window(app._window);

	dialog.lpstrFile = path;
	dialog.nMaxFile = sizeof(path);

	// Get CWD as start directory
	if (GetCurrentDirectoryA(MAX_PATH, dir))
	{
		dialog.lpstrInitialDir = dir;
	}

	dialog.lpstrFilter = filter;
	dialog.nFilterIndex = 1;

	dialog.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&dialog))
	{
		return dialog.lpstrFile;
	}

	return "";
}

#endif
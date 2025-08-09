#pragma once
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "Util/Texture.h"
#include "UI/PalettesUI.h"
#include "UI/PatternTablesUI.h"
#include "UI/DisassemblerUI.h"
#include "UI/SystemControl.h"
#include "UI/BreakpointsUI.h"
#include "Memory/Cartridge.h"
#include "System.h"

class Application
{
public:
	Application();
	~Application() = default;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

public:
	void Run();

	void SetCartridge(const char* filepath);

private:
	void RenderUI();

	void RenderCartridge();
	void RenderLog();

public:
	friend void ResizeCallback(GLFWwindow* window, int width, int height);

private:
	const char* _windowTitle = "NES Emulator";
	int _windowWidth = 1280;
	int _windowHeight = 800;
	GLFWwindow* _window = nullptr;

	Cartridge _cart;
	System _system;

	int _hexdumpPageCPU = 0;
	int _hexdumpPagePPU = 0;
	
	int _framebufferScale = 1;
	int _patternTableScale = 2;

	PalettesUI _palettes;
	PatternTablesUI _patternTables;
	SystemControl _control;
	BreakpointsUI _breakpoints;

	Texture _framebufferTex;


	char _path[256] = { 0 };

	bool _logIncludeDebug = true;
	bool _logIncludeInfo = true;
	bool _logIncludeWarn = true;
	bool _logIncludeError = true;

	std::vector<std::pair<std::string, LogLevel>> _log;
};


#pragma once
#include <utility>
#include <array>
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "Image.h"
#include "Texture.h"
#include "Cartridge.h"
#include "System.h"

class Application
{
public:

	friend class FileDialogs;

	static Application& Get()
	{
		static Application app;
		return app;
	}

	void Run();

	void SetCartridge(const char* filepath);

private:
	void RenderUI();
	
	// TODO: evaluate use of pointers oppssed to const ref
	static void RenderHexdump(const MemoryMap* memory, const char* title, int* page);
	static void RenderIOPort(const IOPort* port, const char* title);
	
	void RenderPatternTables();
	void RenderPalettes();
	void RenderDisassembly();
	void RenderControl();
	void RenderBreakpoints();
	void RenderCartridge();
	void RenderCPURegisters();
	void RenderPPURegisters();
	void RenderLog();

private:
	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

	static void ResizeCallback(GLFWwindow*, int width, int height);

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

	bool _updateTable = false;
	int _patternTablePaletteIndex[2] = { 0 , 0 };
	Image _patternTable[2];
	Texture _patternTableTex[2];

	struct PaletteEntry
	{
		static constexpr size_t NUM_COLORS = 4;
		
		std::string id[NUM_COLORS];
		float colorNorm[NUM_COLORS][3];
	};

	std::array<PaletteEntry, 8> _paletteEntry;

	Texture _framebufferTex;

	char _breakpointText[8];
	bool _enableBreakpoints = true;

	char _path[256] = { 0 };

	bool _systemRun = false;


	bool _logIncludeDebug = true;

	bool _logIncludeInfo = true;
	bool _logIncludeWarn = true;
	bool _logIncludeError = true;

	std::vector<std::pair<std::string, LogLevel>> _log;

	
};


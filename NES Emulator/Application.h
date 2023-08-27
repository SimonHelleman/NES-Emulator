#pragma once
#include <GLFW/glfw3.h>
#include "Image.h"
#include "Texture.h"
#include "Cartridge.h"
#include "System.h"

class Application
{
public:
	static Application& Get()
	{
		return s_instance;
	}

	void Run();

private:
	void RenderUI();
	void RenderHexdump();
	void RenderPatternTables();
	void RenderDisassembly();
	void RenderControl();
	void RenderCPURegisters();

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
	int _windowHeight = 720;
	GLFWwindow* _window = nullptr;

	Cartridge _cart;
	System _system;

	int _hexdumpPage = 0;
	
	int _framebufferScale = 1;
	int _patternTableScale = 2;

	int _patternTablePaletteIndex[2] = { 0 , 0 };
	Image _patternTable[2];
	Texture _patternTableTex[2];

	Texture _framebufferTex;

	bool _systemRun = false;

	static Application s_instance;
};


#include <cstdint>
#include <cstring>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include "Application.h"


Application Application::s_instance;

Application::Application()
	: _cart("nestest.nes"), _system(_cart)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW\n";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_windowWidth, _windowHeight, _windowTitle, nullptr, nullptr);

	if (!_window)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";
		return;
	}

	glViewport(0, 0, _windowWidth, _windowHeight);
	glfwSetFramebufferSizeCallback(_window, ResizeCallback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// io.IniFilename = NULL;


	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}


	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	_system.Reset();

	const Image& framebuffer = _system.GetPPU()->GetFramebuffer();
	_framebufferTex = Texture(framebuffer, Texture::Wrapping::Repeat, Texture::Filtering::Nearest);

	PPU::Palette patternTable1Palette = _system.GetPPU()->GetPalette(_patternTablePaletteIndex[0]);
	PPU::Palette patternTable2Palette = _system.GetPPU()->GetPalette(_patternTablePaletteIndex[1]);
	_patternTable[0] = _system.GetPPU()->GetPatternTable(0, patternTable1Palette);
	_patternTable[1] = _system.GetPPU()->GetPatternTable(1, patternTable2Palette);
	_patternTableTex[0] = Texture(_patternTable[0], Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
	_patternTableTex[1] = Texture(_patternTable[1], Texture::Wrapping::Repeat, Texture::Filtering::Nearest);

	for (size_t i = 0; i < _paletteEntry.size(); ++i)
	{
		for (int color = 0; color < 3; ++color)
		{
			char buf[16];
			snprintf(buf, 16, "Palette %ld.%d", (int)i, color);
			_paletteEntry[i].id[color] = buf;
		}
	}
}

void Application::Run()
{
	while (!glfwWindowShouldClose(_window))
	{
		for (size_t i = 0; i < 50; ++i)
		//for (size_t i = 0; i < 200; ++i)
		{
			_system.Update();
		}
		glfwPollEvents();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderUI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContextBackup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(currentContextBackup);
		}
		glfwSwapBuffers(_window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void Application::RenderUI()
{
	RenderHexdump();

	ImGui::Begin("PPU Framebuffer");
	{
		const Image& framebuffer = _system.GetPPU()->GetFramebuffer();
		_framebufferTex.Update(framebuffer);
		ImGui::Image((void*)(intptr_t)_framebufferTex.TextureId(), ImVec2((float)framebuffer.Width() * _framebufferScale, (float)framebuffer.Height() * _framebufferScale));
	}
	ImGui::End();

	RenderPatternTables();
	RenderPalettes();
	RenderDisassembly();
	RenderControl();
	RenderCPURegisters();
}

void Application::RenderHexdump()
{
	ImGui::Begin("Hexdump");
	{
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.0f, 0.0f });
		ImGui::SliderInt("Page", &_hexdumpPage, 0, 0xff, "%02x");
		if (ImGui::BeginTable("hexdump", 17))
		{
			uint16_t dumpStart = _hexdumpPage * 256;
			uint16_t addr = dumpStart;
			do
			{
				if (addr % 16 == 0)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%04x:", addr);
				}
				ImGui::TableNextColumn();
				ImGui::Text("%02x", _system.CPUMemory()->Read(addr));
				++addr;
			} while (addr && addr < dumpStart + 256);

			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
	}
	ImGui::End();
}

void Application::RenderPatternTables()
{
	ImGui::Begin("Pattern Tables");
	{
		if (ImGui::SliderInt("Left Palette", &_patternTablePaletteIndex[0], 0, 7, "%d"))
		{
			PPU::Palette palette = _system.GetPPU()->GetPalette(_patternTablePaletteIndex[0]);
			_patternTable[0] = _system.GetPPU()->GetPatternTable(0, palette);
			//patternTable1Tex.Update(patternTable1);
			_patternTableTex[0] = Texture(_patternTable[0], Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
		}

		if (ImGui::SliderInt("Right Palette", &_patternTablePaletteIndex[1], 0, 7, "%d"))
		{
			PPU::Palette palette = _system.GetPPU()->GetPalette(_patternTablePaletteIndex[1]);
			_patternTable[1] = _system.GetPPU()->GetPatternTable(1, palette);
			//patternTable2Tex.Update(patternTable2);
			_patternTableTex[1] = Texture(_patternTable[1], Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
		}

		ImGui::Image((void*)(intptr_t)_patternTableTex[0].TextureId(), ImVec2((float)_patternTable[0].Width() * _patternTableScale, (float)_patternTable[1].Height() * _patternTableScale));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)_patternTableTex[1].TextureId(), ImVec2((float)_patternTable[1].Width() * _patternTableScale, (float)_patternTable[1].Height() * _patternTableScale));
	}
	ImGui::End();
}

void Application::RenderPalettes()
{
	ImGui::Begin("Palettes");
	{
		for (size_t i = 0; i < _paletteEntry.size(); ++i)
		{
			PPU::Palette palette = _system.GetPPU()->GetPalette(i);

			for (int color = 0; color < 4; ++color)
			{
				_paletteEntry[i].colorNorm[color][0] = palette.color[color].r / 255.0f;
				_paletteEntry[i].colorNorm[color][1] = palette.color[color].g / 255.0f;
				_paletteEntry[i].colorNorm[color][2] = palette.color[color].b / 255.0f;
 
				ImGui::ColorEdit3(_paletteEntry[i].id[color].c_str(), _paletteEntry[i].colorNorm[color], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				if (color != 3) ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}

void Application::RenderDisassembly()
{
	std::vector<Disassembler::Instruction> instruction = _system.GetDissembler()->GetInstrctionCache();
	ImGui::Begin("Disassembly");
	{
		if (ImGui::BeginTable("disassembly", 2))
		{
			for (int i = 0; i < instruction.size(); ++i)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%04x", instruction[i].address);
				ImGui::TableNextColumn();
				ImGui::Text(Disassembler::GetDisassemblyLine(instruction[i]).c_str());
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();
}

void Application::RenderControl()
{
	ImGui::Begin("Control");
	{
		ImGui::Text("Cycles: %ld", _system.CycleCount());

		if (ImGui::Checkbox("Run", &_systemRun))
		{
			_system.Run(_systemRun);
		}

		if (ImGui::Button("Clock Step"))
		{
			_system.ClockStep();
		}

		if (ImGui::Button("Instruction Step"))
		{
			_system.InstructionStep();
		}

		if (ImGui::Button("Frame Step"))
		{
			_system.FrameStep();
		}
	}
	ImGui::End();
}

void Application::RenderCPURegisters()
{
	ImGui::Begin("CPU Registers");
	{
		ImGui::Text("A:  %02x", _system.GetCPU()->GetA());
		ImGui::Text("X:  %02x", _system.GetCPU()->GetX());
		ImGui::Text("Y:  %02x", _system.GetCPU()->GetY());
		ImGui::Text("SP: %02x", _system.GetCPU()->GetSP());
		ImGui::Text("PC: %04x", _system.GetCPU()->GetProgramCounter());

		char flags[9];
		uint8_t status = _system.GetCPU()->GetStatusReg();
		flags[0] = status & CPU::STATUS_N ? 'N' : '-';
		flags[1] = status & CPU::STATUS_V ? 'V' : '-';
		flags[2] = status & CPU::STATUS_5 ? '5' : '-';
		flags[3] = status & CPU::STATUS_B ? 'B' : '-';
		flags[4] = status & CPU::STATUS_D ? 'D' : '-';
		flags[5] = status & CPU::STATUS_I ? 'I' : '-';
		flags[6] = status & CPU::STATUS_Z ? 'Z' : '-';
		flags[7] = status & CPU::STATUS_C ? 'C' : '-';
		flags[8] = '\0';

		ImGui::Text("Status: %02x [%s]", status, flags);
	}
	ImGui::End();
}

Application::~Application()
{
}

void Application::ResizeCallback(GLFWwindow*, int width, int height)
{
	Application& app = Application::Get();
	app._windowWidth = width;
	app._windowHeight = height;
	glViewport(0, 0, width, height);
}
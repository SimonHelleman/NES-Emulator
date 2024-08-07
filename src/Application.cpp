#include <cstdint>
#include <cstring>
#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include "FileDialogs.h"
#include "Application.h"

Application::Application()
	: _cart(), _system(_cart)
{
	Logger& logger = Logger::Get();
	logger.AddOutput(LogLevel::Debug | LogLevel::Info, Logger::StandardOutput);
	logger.AddOutput(LogLevel::Warning | LogLevel::Error, Logger::StandardErrorOutput);

	logger.AddOutput(LogLevel::All, [this](std::string message, LogLevel level) {
		_log.emplace_back(message, level);
	});

	LOG_DEBUG("debug message");
	INFO("info message");
	WARN("warning message");
	ERROR("error message");

	if (!glfwInit())
	{
		ERROR_FL("Failed to init GLFW");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_windowWidth, _windowHeight, _windowTitle, nullptr, nullptr);

	if (!_window)
	{
		ERROR_FL("Failed to create GLFW window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ERROR_FL("Failed to initialize GLAD");
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
			snprintf(buf, 16, "Palette %d.%d", (int)i, color);
			_paletteEntry[i].id[color] = buf;
		}
	}
}

void Application::Run()
{
	//std::string openPath = FileDialogs::OpenFileDialog("NES ROM (*.nes)\0*.nes\0");
	//INFO(openPath);

	_system.EnableBreakpoints(true);
	while (!glfwWindowShouldClose(_window))
	{
		//for (size_t i = 0; i < 50; ++i)
		//for (size_t i = 0; i < 200; ++i)
		// it turns out it takes ~90000 cycles per frame
		// will use the approx for now
		for (size_t i = 0; i <= 90000; ++i)
		{
			_system.Update();
			_systemRun = _system.IsRunning();
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

void Application::SetCartridge(const char* filepath)
{
	_cart = Cartridge(filepath);
	_system.ChangeCartridge(_cart);
}

void Application::RenderUI()
{
	RenderHexdump(_system.CPUMemory(), "CPU Hexdump", &_hexdumpPageCPU);
	RenderHexdump(_system.PPUMemory(), "PPU Hexdump", &_hexdumpPagePPU);

	RenderIOPort(_system.GetOutputPort(), "IO Port: OUT");
	RenderIOPort(_system.GetInputPort(0), "IO Port: IN0");
	RenderIOPort(_system.GetInputPort(1), "IO Port: IN1");

	ImGui::Begin("PPU Framebuffer");
	{
		const Image& framebuffer = _system.GetPPU()->GetFramebuffer();
		_framebufferTex.Update(framebuffer);
		ImGui::Image((void*)(intptr_t)_framebufferTex.TextureId(), ImVec2((float)framebuffer.Width() * _framebufferScale, (float)framebuffer.Height() * _framebufferScale));
	}
	ImGui::End();

	ImGui::Begin("Debug");
	{
		if (ImGui::Button("Write Disassembly"))
		{
			std::string path;

			if (FileDialogs::Available())
			{
				path = FileDialogs::SaveFileDialog("Assembly Source (*.s)\0*.s\0");
				if (!path.empty())
				{
					_system.GetDissembler()->WriteToFile(path.c_str());
				}
			}
			else
			{
				_system.GetDissembler()->WriteToFile("disassembly.s");
			}

		}
	}
	ImGui::End();

	RenderPalettes();
	RenderPatternTables();
	RenderDisassembly();
	RenderControl();
	RenderBreakpoints();
	RenderCartridge();
	RenderCPURegisters();
	RenderPPURegisters();
	RenderLog();
}

void Application::RenderHexdump(const MemoryMap* memory, const char* title, int* page)
{
	ImGui::Begin(title);
	{
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.0f, 0.0f });
		ImGui::SliderInt("Page", page, 0, 0xff, "%02x");
		if (ImGui::BeginTable("hexdump", 17))
		{
			uint16_t dumpStart = *page * 256;
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
				ImGui::Text("%02x", memory->Read(addr, true));
				++addr;
			} while (addr && addr < dumpStart + 256);

			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
	}
	ImGui::End();
}

void Application::RenderIOPort(const IOPort* port, const char* title)
{
	ImGui::Begin(title);
	{
		const char* directionText = port->Direction() == PortDirection::Input ? 
			"Direction: Input" : "Direction: Output";
		
		ImGui::Text(directionText);

		char bits[12];
		bits[0] = '[';
		const int numPins = port->Direction() == PortDirection::Input ? 5 : 3;
		int i;
		for (i = 1; i < numPins + 1; ++i)
		{
			bits[i] = port->GetPin(i - 1) ? 'H' : 'L';
		}
		bits[i++] = ']';
		bits[i] = '\0';

		ImGui::Text(bits);
	}
	ImGui::End();
}

void Application::RenderPalettes()
{
	ImGui::Begin("Palettes");
	{
		for (int i = 0; i < _paletteEntry.size(); ++i)
		{
			PPU::Palette palette = _system.GetPPU()->GetPalette(i);

			for (int color = 0; color < 4; ++color)
			{
				float newR = palette.color[color].r / 255.0f;
				float newG = palette.color[color].g / 255.0f;
				float newB = palette.color[color].b / 255.0f;

				float oldR = _paletteEntry[i].colorNorm[color][0];
				float oldG = _paletteEntry[i].colorNorm[color][1];
				float oldB = _paletteEntry[i].colorNorm[color][2];

				if (newR != oldR || newG != oldG || newB != oldB)
				{
					_paletteEntry[i].colorNorm[color][0] = newR;
					_paletteEntry[i].colorNorm[color][1] = newG;
					_paletteEntry[i].colorNorm[color][2] = newB;
					_updateTable = true;
				}
 
				ImGui::ColorEdit3(_paletteEntry[i].id[color].c_str(), _paletteEntry[i].colorNorm[color], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				if (color != 3) ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}

void Application::RenderPatternTables()
{
	ImGui::Begin("Pattern Tables");
	{
		if (ImGui::SliderInt("Left Palette", &_patternTablePaletteIndex[0], 0, 7, "%d") || _updateTable)
		{
			PPU::Palette palette = _system.GetPPU()->GetPalette(_patternTablePaletteIndex[0]);
			_patternTable[0] = _system.GetPPU()->GetPatternTable(0, palette);
			_patternTableTex[0].Update(_patternTable[0]);
		}

		if (ImGui::SliderInt("Right Palette", &_patternTablePaletteIndex[1], 0, 7, "%d") || _updateTable)
		{
			PPU::Palette palette = _system.GetPPU()->GetPalette(_patternTablePaletteIndex[1]);
			_patternTable[1] = _system.GetPPU()->GetPatternTable(1, palette);
			_patternTableTex[1].Update(_patternTable[1]);
		}

		ImGui::Image((void*)(intptr_t)_patternTableTex[0].TextureId(), ImVec2((float)_patternTable[0].Width() * _patternTableScale, (float)_patternTable[1].Height() * _patternTableScale));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)_patternTableTex[1].TextureId(), ImVec2((float)_patternTable[1].Width() * _patternTableScale, (float)_patternTable[1].Height() * _patternTableScale));
	}
	ImGui::End();

	_updateTable = false;
}

void Application::RenderDisassembly()
{
	std::vector<Disassembler::Instruction> instruction = _system.GetDissembler()->GetInstrctionCache();
	ImGui::Begin("Disassembly");
	{
		if (ImGui::BeginTable("disassembly", 2))
		{
			for (size_t i = 0; i < instruction.size(); ++i)
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

		ImGui::Checkbox("Run", &_systemRun);
		_system.Run(_systemRun);

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

void Application::RenderBreakpoints()
{
	ImGui::Begin("Breakpoints");
	{
		if (ImGui::Checkbox("Enable", &_enableBreakpoints))
		{
			_system.EnableBreakpoints(_enableBreakpoints);
		}

		// Setting buffer size to 5 just to limit it to a 4 digit number
		ImGui::InputText("Address", _breakpointText, 5, ImGuiInputTextFlags_CharsHexadecimal);

		if (ImGui::Button("Add Breakpoint") && strlen(_breakpointText) > 0)
		{
			uint16_t addr;
			std::stringstream ss;
			ss << std::hex << _breakpointText;
			ss >> addr;
			_system.AddBreakpoint(addr);
		}

		for (uint16_t b : _system.GetBreakpoints())
		{
			ImGui::Text("%04x", b);
			ImGui::SameLine();
			char buf[16];
			snprintf(buf, 16, "Delete##%d", b);
			if (ImGui::Button(buf))
			{
				_system.RemoveBreakpoint(b);
				break; // Sneaky break. A tad hacky but it works
			}
		}
	}
	ImGui::End();
}

void Application::RenderCartridge()
{
	ImGui::Begin("Cartridge");
	{
		if (FileDialogs::Available())
		{
			if (ImGui::Button("Open ROM"))
			{
				std::string filepath = FileDialogs::OpenFileDialog("NES ROM (*.nes)\0*.nes\0");
				if (!filepath.empty())
				{
					_cart = Cartridge(filepath.c_str());
					_system.ChangeCartridge(_cart);
				}
			}
		}
		else // "Peasant" mode
		{
			ImGui::InputText("path", _path, 256);
			ImGui::SameLine();
			if (ImGui::Button("Open ROM"))
			{
				//LOG_DEBUG(path);
				_cart = Cartridge(_path);
				_system.ChangeCartridge(_cart);
			}
		}

		ImGui::SameLine();
		ImGui::Text("%s", _cart.Name().c_str());
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

void Application::RenderPPURegisters()
{
	ImGui::Begin("PPU Registers");
	{
		ImGui::Text("Addr: %04x", _system.GetPPU()->AddressReg());
		ImGui::Text("Data: %02x", _system.GetPPU()->DataReg());
	
		char flags[9];
		uint8_t control = _system.GetPPU()->ControlReg();
		flags[0] = control & PPU::CONTROL_V ? 'V' : '-';
		flags[1] = control & PPU::CONTROL_P ? 'P' : '-';
		flags[2] = control & PPU::CONTROL_H ? 'H' : '-';
		flags[3] = control & PPU::CONTROL_B ? 'B' : '-';
		flags[4] = control & PPU::CONTROL_S ? 'S' : '-';
		flags[5] = control & PPU::CONTROL_I ? 'I' : '-';
		flags[6] = control & PPU::CONTROL_2 ? '2' : '-';
		flags[7] = control & PPU::CONTROL_1 ? '1' : '-';
		flags[8] = '\0';
		ImGui::Text("Control: %02x [%s]", control, flags);
	}
	ImGui::End();
}

void Application::RenderLog()
{
	ImGui::Begin("Log");
	{
#ifdef DEBUG
		ImGui::Checkbox("Debug", &_logIncludeDebug);
		ImGui::SameLine();
#endif
		ImGui::Checkbox("Info", &_logIncludeInfo);
		ImGui::SameLine();

		ImGui::Checkbox("Warnings", &_logIncludeWarn);
		ImGui::SameLine();

		ImGui::Checkbox("Error", &_logIncludeError);

		ImGui::BeginChild("TableScrollingRegion", ImVec2(0, 0), false);
		for (size_t i = 0; i < _log.size(); ++i)
		{
			if (_log[i].second & LogLevel::Debug && _logIncludeDebug)
			{
				ImGui::Text(_log[i].first.c_str());
			}
			else if (_log[i].second & LogLevel::Info && _logIncludeInfo)
			{
				ImGui::Text(_log[i].first.c_str());
			}
			else if (_log[i].second & LogLevel::Warning && _logIncludeWarn)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.647f, 0.0f, 1.0f));
				ImGui::Text(_log[i].first.c_str());
				ImGui::PopStyleColor();
			}
			else if (_log[i].second & LogLevel::Error && _logIncludeError)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				ImGui::Text(_log[i].first.c_str());
				ImGui::PopStyleColor();
			}
			ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();
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
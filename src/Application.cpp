#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include "Util/FileDialogs.h"
#include "UI/RegisterDisplays.h"
#include "UI/HexdumpDisplay.h"
#include "UI/IODisplays.h"
#include "Application.h"

Application::Application()
	: _cart(), _system(_cart), _palettes(*_system.GetPPU()), _patternTables(*_system.GetPPU()),
	  _control(_system), _breakpoints(_system)
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

	_patternTables.InitTextures();

	_system.Reset();

	const Image& framebuffer = _system.GetPPU()->GetFramebuffer();
	_framebufferTex = Texture(framebuffer, Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
}

void Application::Run()
{
	//std::string openPath = FileDialogs::OpenFileDialog("NES ROM (*.nes)\0*.nes\0");
	//INFO(openPath);

	_system.EnableBreakpoints(true);
	while (!glfwWindowShouldClose(_window))
	{

		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::Start, glfwGetKey(_window, GLFW_KEY_ENTER));
		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::Select, glfwGetKey(_window, GLFW_KEY_T));
		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::A, glfwGetKey(_window, GLFW_KEY_A));
		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::B, glfwGetKey(_window, GLFW_KEY_B));
		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::Up, glfwGetKey(_window, GLFW_KEY_UP));
		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::Down, glfwGetKey(_window, GLFW_KEY_DOWN));
		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::Left, glfwGetKey(_window, GLFW_KEY_LEFT));
		_system.GetJoypad(0)->SetButton(StandardJoypad::Button::Right, glfwGetKey(_window, GLFW_KEY_RIGHT));

		//for (size_t i = 0; i < 50; ++i)
		//for (size_t i = 0; i < 200; ++i)
		// it turns out it takes ~90000 cycles per frame
		// will use the approx for now
		for (size_t i = 0; i <= 90000; ++i)
		{
			_system.Update();
			_control.SetRunCheckbox(_system.IsRunning());
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
	RenderHexdump(*_system.CPUMemory(), "CPU Hexdump", &_hexdumpPageCPU);
	RenderHexdump(*_system.PPUMemory(), "PPU Hexdump", &_hexdumpPagePPU);

	RenderIOPort(*_system.GetOutputPort(), "IO Port: OUT");
	RenderIOPort(*_system.GetInputPort(0), "IO Port: IN0");
	RenderIOPort(*_system.GetInputPort(1), "IO Port: IN1");

	RenderStdJoypad(*_system.GetJoypad(0), "JOY0");
	RenderStdJoypad(*_system.GetJoypad(1), "JOY1");

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

	_palettes.Render();
	_patternTables.Render(_palettes.HasChanged(), _patternTableScale);
	_breakpoints.Render();
	_control.Render();
	RenderDisassembler(*_system.GetDissembler());
	RenderCartridge();
	RenderCPURegisters(*_system.GetCPU());
	RenderPPURegisters(*_system.GetPPU());
	RenderLog();
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
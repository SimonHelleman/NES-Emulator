#include <iostream>
#include <fstream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RAMOnlyMemMap.h"
#include "CPU.h"

static int s_windowWidth = 1280;
static int s_windowHeight = 720;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	s_windowWidth = width;
	s_windowHeight = height;
	glViewport(0, 0, width, height);
}

int main()
{	
	std::cout << "hello world\n";

	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(s_windowWidth, s_windowHeight, "NES Emulator", nullptr, nullptr);

	if (!window)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, s_windowWidth, s_windowHeight);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	bool showDemoWindow = true;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("NES Emulator");
		ImGui::Text("Hello world from ImGui :)");
		ImGui::End();

		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContextBackup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(currentContextBackup);
		}
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

/*
	RAMOnlyMemMap memory;

	memory.LoadFromFile("adc_stress_test.bin");

	std::ofstream memDumpFile("hexdump.txt");
	memDumpFile << memory.HexDump() << '\n';

	memDumpFile.close();
			
	Disassembler disassembler = Disassembler(memory);
	CPU cpu = CPU(memory, &disassembler);
	cpu.Reset();

	while (cpu.GetCurrentInstruction().mnemonic != "NOP")
	{
		cpu.Clock();
	}

	memDumpFile.open("hexdump.txt");
	memDumpFile << memory.HexDump();
	memDumpFile.close();
*/
	return 0;
}
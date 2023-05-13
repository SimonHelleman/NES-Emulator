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
static GLFWwindow* s_window;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	s_windowWidth = width;
	s_windowHeight = height;
	glViewport(0, 0, width, height);
}

int InitUI()
{

	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	s_window = glfwCreateWindow(s_windowWidth, s_windowHeight, "NES Emulator", nullptr, nullptr);

	if (!s_window)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(s_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, s_windowWidth, s_windowHeight);
	glfwSetFramebufferSizeCallback(s_window, FramebufferSizeCallback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	//io.IniFilename = NULL;

	
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(s_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	return 0;
}

int main()
{	
	std::cout << "hello world\n";

	int error = InitUI();

	if (error)
	{
		return error;
	}

	bool showDemoWindow = false;

	RAMOnlyMemMap memory;

	memory.LoadFromFile("adc_stress_test.bin");

	int maxDump = 1;
	while (!glfwWindowShouldClose(s_window))
	{
		glfwPollEvents();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		// This is a definite "refactor later" block of code
		// Not too sure how to abstract away the UI until I have more
		// of the system going
		ImGui::Begin("Hexdump");
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.0f, 0.0f });
		ImGui::SliderInt("Dump End", &maxDump, 1, MemoryMap::ADRESSABLE_RANGE, "%04x");
		if (ImGui::BeginTable("hexdump", 17))
		{
			for (size_t i = 0; i < maxDump; ++i)
			{
				if (i % 16 == 0)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%04x:", i);
				}
				ImGui::TableNextColumn();
				ImGui::Text("%02x", memory.Read(i));
			}
			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
		ImGui::End();

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
		glfwSwapBuffers(s_window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

/*
	
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
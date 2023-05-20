#include <iostream>
#include <fstream>
#include <array>
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

	int error;
	if (error = InitUI())
	{
		return error;
	}

	bool showDemoWindow = true;

	RAMOnlyMemMap memory;
	memory.LoadFromFile("adc_stress_test.bin");

	Disassembler disassembler = Disassembler(memory);
	CPU cpu = CPU(memory, &disassembler);
	cpu.Reset();

	int numInstructionsInTable = 0;
	bool instructionAdded = false;
	std::array<Disassembler::Instruction, 10> instTable;

	int maxDump = 1;
	bool continuousRun = true;
	bool clockUntilFinished = false;
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
		{
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.0f, 0.0f });
			ImGui::SliderInt("Dump End", &maxDump, 1, MemoryMap::ADRESSABLE_RANGE, "%04x");
			if (ImGui::BeginTable("hexdump", 17))
			{
				for (uint16_t i = 0; i < maxDump; ++i)
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
		}
		ImGui::End();

		ImGui::Begin("PPU Framebuffer");
		//ImGui::Image()
		ImGui::End();

		//if (cpu.GetCurrentState() == CPU::State::AddressingMode && !instructionAdded)
		if (cpu.IsInstFinished() && !instructionAdded)
		{
			if (numInstructionsInTable < instTable.size())
			{
				instTable[numInstructionsInTable++] = disassembler.GetLatestInstruction();
			}
			else
			{
				for (size_t i = 0; i < instTable.size() - 1; ++i)
				{
					instTable[i] = instTable[i + 1];
				}
				instTable[instTable.size() - 1] = disassembler.GetLatestInstruction();
			}
			instructionAdded = true;
		}

		if (!cpu.IsInstFinished()) instructionAdded = false;

		ImGui::Begin("Disassembly");
		{
			if (ImGui::BeginTable("disassembly", 2))
			{
				for (int i = 0; i < numInstructionsInTable; ++i)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%04x", instTable[i].address);
					ImGui::TableNextColumn();
					ImGui::Text(Disassembler::GetDisassemblyLine(instTable[i]).c_str());
				}
				ImGui::EndTable();
			}
		}
		ImGui::End();

		ImGui::Begin("Control");
		{
			
			ImGui::Checkbox("Run", &continuousRun);

			if (ImGui::Button("Clock Step") || continuousRun)
			{
				cpu.Clock();
			}

			if (ImGui::Button("Instruction Step") && !continuousRun)
			{
				clockUntilFinished = true;
				cpu.Clock();
			}

			if (clockUntilFinished)
			{
				if (!cpu.IsInstFinished())
				{
					cpu.Clock();
				}
				else
				{
					clockUntilFinished = false;
				}
			}
			ImGui::End();
		}

		ImGui::Begin("CPU Registers");
		{
			ImGui::Text("A:  %02x", cpu.GetA());
			ImGui::Text("X:  %02x", cpu.GetX());
			ImGui::Text("Y:  %02x", cpu.GetY());
			ImGui::Text("SP: %02x", cpu.GetSP());
			ImGui::Text("PC: %04x", cpu.GetProgramCounter());

			char flags[9];
			uint8_t status = cpu.GetStatusReg();
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
	return 0;
}
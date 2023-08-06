#include <iostream>
#include <fstream>
#include <array>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Image.h"
#include "Texture.h"
#include "Cartridge.h"
#include "Mapper0.h"
#include "RAMOnlyMemMap.h"
#include "CPU.h"
#include "PPU.h"

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

	// io.IniFilename = NULL;

	
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

	bool showDemoWindow = false;


	Cartridge cart = Cartridge("nestest.nes");

	PPUMapper0 ppuMemory = PPUMapper0(cart.CharacterROM(), cart.MirroringMode());

	PPU ppu = PPU(ppuMemory);
	
	CPUMapper0 memory = CPUMapper0(&ppu, cart.ProgramROM(), cart.ProgramROMSize());

	Disassembler disassembler = Disassembler(memory);
	CPU cpu = CPU(memory, &disassembler);
	cpu.Reset();

	int numInstructionsInTable = 0;
	bool instructionAdded = false;
	std::array<Disassembler::Instruction, 10> instTable;

	int dumpPage = 0;

	bool continuousRun = false;
	bool clockUntilFinished = false;

	//PPU::Pallet pallet = {
	//	{ 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }
	//};


	int patternTable1PalletIndex = 0;
	int patternTable2PalletIndex = 0;
	PPU::Pallet patternTable1Pallet = ppu.GetPallet(patternTable1PalletIndex);
	PPU::Pallet patternTable2Pallet = ppu.GetPallet(patternTable2PalletIndex);

	Image patternTable1 = ppu.GetPatternTable(0, patternTable1Pallet);
	Image patternTable2 = ppu.GetPatternTable(1, patternTable2Pallet);
	const Image& framebuffer = ppu.GetFramebuffer();

	Texture patternTable1Tex = Texture(patternTable1, Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
	Texture patternTable2Tex = Texture(patternTable2, Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
	Texture framebufferTex = Texture(framebuffer, Texture::Wrapping::Repeat, Texture::Filtering::Nearest);

	int framebufferScale = 1;
	int patternTableScale = 2;
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
			ImGui::SliderInt("Page", &dumpPage, 0, 0xff, "%02x");
			if (ImGui::BeginTable("hexdump", 17))
			{
				uint16_t dumpStart = dumpPage * 256;
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
					ImGui::Text("%02x", memory.Read(addr));
					++addr;
				} while (addr && addr < dumpStart + 256);

				ImGui::EndTable();
			}
			ImGui::PopStyleVar();
		}
		ImGui::End();

		ImGui::Begin("PPU Framebuffer");
		{
			ImGui::Image((void*)(intptr_t)framebufferTex.TextureId(), ImVec2((float)framebuffer.Width() * framebufferScale, (float)framebuffer.Height() * framebufferScale));
		}
		ImGui::End();

		ImGui::Begin("Pattern Tables");
		{
			if (ImGui::SliderInt("Left Pallet", &patternTable1PalletIndex, 0, 7, "%d"))
			{
				patternTable1Pallet = ppu.GetPallet(patternTable1PalletIndex);
				patternTable1 = ppu.GetPatternTable(0, patternTable1Pallet);
				patternTable1Tex.Update(patternTable1);
			}
			
			if (ImGui::SliderInt("Right Pallet", &patternTable2PalletIndex, 0, 7, "%d"))
			{
				patternTable2Pallet = ppu.GetPallet(patternTable2PalletIndex);
				patternTable2 = ppu.GetPatternTable(1, patternTable2Pallet);
				patternTable2Tex.Update(patternTable2);
			}

			ImGui::Image((void*)(intptr_t)patternTable1Tex.TextureId(), ImVec2((float)patternTable1.Width() * patternTableScale, (float)patternTable1.Height() * patternTableScale));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)patternTable2Tex.TextureId(), ImVec2((float)patternTable2.Width() * patternTableScale, (float)patternTable2.Height() * patternTableScale));
		}
		ImGui::End();

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
#include <imgui.h>
#include "RegisterDisplays.h"

void RenderCPURegisters(const CPU& cpu)
{
	ImGui::Begin("CPU Registers");
	{
		ImGui::Text("A:  %02x", cpu.AReg());
		ImGui::Text("X:  %02x", cpu.XReg());
		ImGui::Text("Y:  %02x", cpu.YReg());
		ImGui::Text("SP: %02x", cpu.StackPtr());
		ImGui::Text("PC: %04x", cpu.ProgramCounter());

		char flags[9];
		const uint8_t status = cpu.StatusReg();
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

void RenderPPURegisters(const PPU& ppu)
{
	ImGui::Begin("PPU Registers");
	{
		ImGui::Text("Addr: %04x", ppu.AddressReg());
		ImGui::Text("Data: %02x", ppu.DataReg());

		char flags[9];
		const uint8_t control = ppu.ControlReg();
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
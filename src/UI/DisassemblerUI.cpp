#include <imgui.h>
#include "DisassemblerUI.h"

void RenderDisassembler(const Disassembler& disassembler)
{
	std::vector<Disassembler::Instruction> instruction = disassembler.InstrctionCache();
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
				ImGui::Text(Disassembler::MakeDisassemblyLine(instruction[i]).c_str());
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();
}
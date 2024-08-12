#include <imgui.h>
#include "HexdumpDisplay.h"

void RenderHexdump(const MemoryMap& memory, const char* title, int* page)
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
				ImGui::Text("%02x", memory.Read(addr, true));
				++addr;
			} while (addr && addr < dumpStart + 256);

			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
	}
	ImGui::End();
}
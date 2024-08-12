#include <sstream>
#include <imgui.h>
#include "BreakpointsUI.h"

void BreakpointsUI::Render()
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
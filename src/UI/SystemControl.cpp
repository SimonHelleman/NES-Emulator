#include <imgui.h>
#include "SystemControl.h"

void SystemControl::Render()
{
	ImGui::Begin("Control");
	{
		ImGui::Text("Cycles: %ld", _system.CycleCount());

		ImGui::Checkbox("Run", &_run);
		_system.Run(_run);

		if (ImGui::Button("Reset"))
		{
			_system.Reset();
		}

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
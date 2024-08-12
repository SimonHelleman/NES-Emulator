#include <imgui.h>
#include "IODisplays.h"

void RenderIOPort(const IOPort& port, const char* title)
{
	ImGui::Begin(title);
	{
		const char* directionText = port.Direction() == PortDirection::Input ?
			"Direction: Input" : "Direction: Output";

		ImGui::Text(directionText);

		char bits[12];
		bits[0] = '[';
		const int numPins = port.Direction() == PortDirection::Input ? 5 : 3;
		int i;
		for (i = 1; i < numPins + 1; ++i)
		{
			bits[i] = port.GetPin(i - 1) ? 'H' : 'L';
		}
		bits[i++] = ']';
		bits[i] = '\0';

		ImGui::Text(bits);

		if (port.Direction() == PortDirection::Input)
		{
			ImGui::Text("OE: %d", port.GetOE());
		}
	}
	ImGui::End();
}

void RenderStdJoypad(const StandardJoypad& controller, const char* title)
{
	ImGui::Begin(title);
	{
		char bitsStr[12];
		bitsStr[0] = '[';
		bitsStr[1] = controller.IsButttonDown(StandardJoypad::Button::A) ? 'A' : '-';
		bitsStr[2] = controller.IsButttonDown(StandardJoypad::Button::B) ? 'B' : '-';
		bitsStr[3] = controller.IsButttonDown(StandardJoypad::Button::Select) ? 'T' : '-';
		bitsStr[4] = controller.IsButttonDown(StandardJoypad::Button::Start) ? 'S' : '-';
		bitsStr[5] = controller.IsButttonDown(StandardJoypad::Button::Up) ? 'U' : '-';
		bitsStr[6] = controller.IsButttonDown(StandardJoypad::Button::Down) ? 'D' : '-';
		bitsStr[7] = controller.IsButttonDown(StandardJoypad::Button::Left) ? 'L' : '-';
		bitsStr[8] = controller.IsButttonDown(StandardJoypad::Button::Right) ? 'R' : '-';
		bitsStr[9] = ']';
		bitsStr[10] = '\0';

		ImGui::Text("Buttons Down: %s", bitsStr);

		bitsStr[1] = controller.ShiftReg() & (1 << 7) ? '1' : '-';
		bitsStr[2] = controller.ShiftReg() & (1 << 6) ? '1' : '-';
		bitsStr[3] = controller.ShiftReg() & (1 << 5) ? '1' : '-';
		bitsStr[4] = controller.ShiftReg() & (1 << 4) ? '1' : '-';
		bitsStr[5] = controller.ShiftReg() & (1 << 3) ? '1' : '-';
		bitsStr[6] = controller.ShiftReg() & (1 << 2) ? '1' : '-';
		bitsStr[7] = controller.ShiftReg() & (1 << 1) ? '1' : '-';
		bitsStr[8] = controller.ShiftReg() & (1 << 0) ? '1' : '-';

		ImGui::Text("Shift Reg: %s", bitsStr);
	}
	ImGui::End();
}

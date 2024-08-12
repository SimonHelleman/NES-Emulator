#include <imgui.h>
#include "PalettesUI.h"

PalettesUI::PalettesUI(const PPU& ppu)
	: _ppu(ppu)
{
	for (size_t i = 0; i < _paletteEntry.size(); ++i)
	{
		for (int color = 0; color < 3; ++color)
		{
			char buf[16];
			snprintf(buf, 16, "Palette %d.%d", (int)i, color);
			_paletteEntry[i].id[color] = buf;
		}
	}
}

void PalettesUI::Render()
{
	ImGui::Begin("Palettes");
	{
		for (int i = 0; i < _paletteEntry.size(); ++i)
		{
			PPU::Palette palette = _ppu.GetPalette(i);

			for (int color = 0; color < 4; ++color)
			{
				float newR = palette.color[color].r / 255.0f;
				float newG = palette.color[color].g / 255.0f;
				float newB = palette.color[color].b / 255.0f;

				float oldR = _paletteEntry[i].colorNorm[color][0];
				float oldG = _paletteEntry[i].colorNorm[color][1];
				float oldB = _paletteEntry[i].colorNorm[color][2];

				if (newR != oldR || newG != oldG || newB != oldB)
				{
					_paletteEntry[i].colorNorm[color][0] = newR;
					_paletteEntry[i].colorNorm[color][1] = newG;
					_paletteEntry[i].colorNorm[color][2] = newB;
					_hasChanged = true;
				}
				else
				{
					_hasChanged = false;
				}

				ImGui::ColorEdit3(_paletteEntry[i].id[color].c_str(), _paletteEntry[i].colorNorm[color], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				if (color != 3) ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}
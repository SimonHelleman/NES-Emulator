#include <imgui.h>
#include "PatternTablesUI.h"

PatternTablesUI::PatternTablesUI(const PPU& ppu)
	: _ppu(ppu)
{
	const PPU::Palette patternTable1Palette = ppu.GetPalette(_patternTablePaletteIndex[0]);
	const PPU::Palette patternTable2Palette = ppu.GetPalette(_patternTablePaletteIndex[1]);

	_patternTable[0] = ppu.GetPatternTable(0, patternTable1Palette);
	_patternTable[1] = ppu.GetPatternTable(1, patternTable2Palette);
}

void PatternTablesUI::InitTextures()
{
	_patternTableTex[0] = Texture(_patternTable[0], Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
	_patternTableTex[1] = Texture(_patternTable[1], Texture::Wrapping::Repeat, Texture::Filtering::Nearest);
}

void PatternTablesUI::Render(bool forceUpdate, int scale)
{
	ImGui::Begin("Pattern Tables");
	{
		if (ImGui::SliderInt("Left Palette", &_patternTablePaletteIndex[0], 0, 7, "%d") || forceUpdate)
		{
			PPU::Palette palette = _ppu.GetPalette(_patternTablePaletteIndex[0]);
			_patternTable[0] = _ppu.GetPatternTable(0, palette);
			_patternTableTex[0].Update(_patternTable[0]);
		}

		if (ImGui::SliderInt("Right Palette", &_patternTablePaletteIndex[1], 0, 7, "%d") || forceUpdate)
		{
			PPU::Palette palette = _ppu.GetPalette(_patternTablePaletteIndex[1]);
			_patternTable[1] = _ppu.GetPatternTable(1, palette);
			_patternTableTex[1].Update(_patternTable[1]);
		}

		ImGui::Image((void*)(intptr_t)_patternTableTex[0].TextureId(), ImVec2((float)_patternTable[0].Width() * scale, (float)_patternTable[1].Height() * scale));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)_patternTableTex[1].TextureId(), ImVec2((float)_patternTable[1].Width() * scale, (float)_patternTable[1].Height() * scale));
	}
	ImGui::End();
}

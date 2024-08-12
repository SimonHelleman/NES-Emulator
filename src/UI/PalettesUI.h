#pragma once
#include <array>
#include <string>
#include "PPU/PPU.h"

class PalettesUI
{
private:
	struct PaletteEntry
	{
		static constexpr size_t NUM_COLORS = 4;

		std::string id[NUM_COLORS];
		float colorNorm[NUM_COLORS][3];
	};

public:
	PalettesUI(const PPU& ppu);

	void Render();

	bool HasChanged() const
	{
		return _hasChanged;
	}

private:
	std::array<PaletteEntry, 8> _paletteEntry;
	const PPU& _ppu;
	bool _hasChanged = false;

};
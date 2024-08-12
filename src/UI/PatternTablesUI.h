#pragma once
#include "Util/Image.h"
#include "Util/Texture.h"
#include "PPU/PPU.h"

class PatternTablesUI
{
public:
	PatternTablesUI(const PPU& ppu);

	// Call after init OpenGL (this is why this is not in the constructor)
	void InitTextures();
	void Render(bool forceUpdate, int scale);

private:
	const PPU& _ppu;
	int _patternTablePaletteIndex[2] = { 0 , 0 };
	Image _patternTable[2];
	Texture _patternTableTex[2];
};
#pragma once
#include "System.h"

class BreakpointsUI
{
public:

	BreakpointsUI(System& sys)
		: _system(sys)
	{

	}

	void Render();

private:
	System& _system;
	char _breakpointText[8];
	bool _enableBreakpoints = true;
};
#pragma once
#include "System.h"

class SystemControl
{
public:
	SystemControl(System& sys)
		: _system(sys), _run(false)
	{

	}
	
	void SetRunCheckbox(bool val)
	{
		_run = val;
	}

	void Render();

private:
	System& _system;
	bool _run;
};
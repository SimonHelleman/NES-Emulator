#include <iostream>
#include "Application.h"

int main(int argc, char** argv)
{
	std::cout << "hello world\n";

	Application& app = Application::Get();

	if (argc == 2)
	{
		app.SetCartridge(argv[1]);
	}

	app.Run();
	return 0;
}

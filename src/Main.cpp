#include <iostream>
#include "Application.h"

int main()
{
	std::cout << "hello world\n";

	Application& app = Application::Get();

	app.Run();
	return 0;
}

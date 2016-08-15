#include <iostream>
#include <stdexcept>

#include <Application.h>

int main()
{
	try
	{
		Application& appInst = Application::GetInstance();
		appInst.Run();
	}
	catch (const std::runtime_error & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

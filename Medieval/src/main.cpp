#include "Application.h"
#include <iostream>
#include <stdexcept>

int main()
{
	try {
		Application& appInst = Application::getInstance();
		appInst.run();
	}catch(const std::runtime_error & e){
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

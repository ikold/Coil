#pragma once

#ifdef CL_PLATFORM_WINDOWS

#include "Logger.h"

extern Coil::Application* Coil::CreatApplication();

int main(int argc, char* argv[])
{
	Coil::Logger::Info("Creating Application");

	auto app = Coil::CreatApplication();
	app->Run();

	delete app;

	return 0;
}

#elif

/*TO DO - others systems*/
int main(int argc, char* gv)
{
	return 0;
}

#endif // CL_PLATFORM_WINDOWS

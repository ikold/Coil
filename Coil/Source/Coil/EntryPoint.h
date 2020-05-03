#pragma once

#ifdef CL_PLATFORM_WINDOWS

extern Coil::Application* Coil::CreatApplication();

int32 main(int32 argc, char8* argv[])
{
	Coil::Logger::Info("Creating Application");

	auto app = Coil::CreatApplication();
	app->Run();

	delete app;

	return 0;
}

#elif CL_PLATFORM_LINUX

extern Coil::Application* Coil::CreatApplication();

int32 main(int32 argc, char8* argv[])
{
	Coil::Logger::Info("Creating Application");

	auto app = Coil::CreatApplication();
	app->Run();

	delete app;

	return 0;
}

#endif

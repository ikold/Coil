#pragma once

#ifdef CL_PLATFORM_WINDOWS

extern Coil::Application* Coil::CreateApplication();

int32 main(int32 argc, char8* argv[])
{
	Coil::Logger::Info("Creating Application");

	auto* app = Coil::CreateApplication();
	app->Run();

	delete app;

	return 0;
}

#endif

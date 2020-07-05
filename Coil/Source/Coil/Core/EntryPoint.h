#pragma once

#ifdef CL_PLATFORM_WINDOWS


int32 main([[maybe_unused]] int32 argc, [[maybe_unused]] char8* argv[])
{
	auto* app = Coil::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

#endif

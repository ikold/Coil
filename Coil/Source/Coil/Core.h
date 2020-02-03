#pragma once


#ifdef CL_PLATFORM_WINDOWS
	#ifdef CL_BUILD_DLL
		#define COIL_API __declspec(dllexport)
	#else
		#define COIL_API __declspec(dllimport)
	#endif // CL_BUILD_DLL
	
#elif CL_PLATFORM_LINUX
	#ifdef CL_BUILD_DLL
		#define COIL_API __attribute__((visibility("default")))
	#else
		#define COIL_API
	#endif // CL_BUILD_DLL
#else

	#error This system is not supported!
#endif
#pragma once


#ifdef CL_PLATFORM_WINDOWS
	#ifdef CL_BUILD_DLL
		#define COIL_API __declspec(dllexport)
	#else
		#define COIL_API __declspec(dllimport)
	#endif // CL_BUILD_DLL
#else
	#error Only Windows is supported!
#endif // CL_PLATFORM_WINDOWS
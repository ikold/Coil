#pragma once


#ifdef CL_ENABLE_ASSERTS
#define CL_ASSERT(x, message) { if (!(x)) { Logger::Error(message); __debugbreak(); } }
#define CL_CORE_ASSERT(x, message) { if (!(x)) { Logger::Error(message); __debugbreak(); } }
#else
#define CL_ASSERT(x, message);
#define CL_CORE_ASSERT(x, message);
#endif

#pragma once

#include <memory>

#ifdef CL_ENABLE_ASSERTS
#define CL_ASSERT(x, message) { if (!(x)) { Logger::Error(message); __debugbreak(); } }
#define CL_CORE_ASSERT(x, message) { if (!(x)) { Logger::Error(message); __debugbreak(); } }
#else
#define CL_ASSERT(x, message);
#define CL_CORE_ASSERT(x, message);
#endif

namespace Coil
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	
	template<typename T>
	using Ref = std::shared_ptr<T>;
}

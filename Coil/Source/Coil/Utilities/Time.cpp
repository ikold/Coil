#include "pch.h"
#include "Time.h"

#include <sstream>
#include <iomanip>


namespace Coil
{
	int64 Time::Query()
	{
#ifdef CL_PLATFORM_WINDOWS
		LARGE_INTEGER timeQuery;
		// QueryPerformanceCounter returns value in counts
		QueryPerformanceCounter(&timeQuery);
		return timeQuery.QuadPart;
#else
		return 0;
#endif
	}

	int64 Time::QueryFrequency()
	{
		// Frequency doesn't change during program runtime and can be cached
		static int64 frequency = QueryFrequencyImpl();
		return frequency;
	}

	RString<> Time::TimestampToString(const Timestamp& timestamp, const RString<>& format)
	{
		CL_PROFILE_FUNCTION_LOW()

		std::stringstream ss;
		ss << std::put_time(std::localtime(&timestamp), format->CString());
		return ss.str().c_str();
	}

	void Time::Tick()
	{
		CL_PROFILE_FUNCTION_HIGH()

		// Updating current frame time
		CurrentFrameTime = Query();

		// Creating converter for tick queries
		static auto queryToMilliseconds = QueryConverter<float32>(Unit::Millisecond);
		
		// Converting to milliseconds
		DeltaTimeV = queryToMilliseconds(CurrentFrameTime - LastFrameTime);

		FpsV = 1000.f / DeltaTimeV;

		LastFrameTime = CurrentFrameTime;
	}

	int64 Time::QueryFrequencyImpl()
	{
#ifdef CL_PLATFORM_WINDOWS
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
#else
		return 1;
#endif
	}
}

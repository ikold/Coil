#include "pch.h"
#include "Time.h"

#include <sstream>
#include <iomanip>


namespace Coil
{
	// Initialization of last frame time point with current time, resulting in first frame time being time from start of program to first call of Time::Tick()
	int64 Time::LastFrameTime = Query();
	int64 Time::CurrentFrameTime;

	float32 Time::DeltaTimeV;
	float32 Time::FpsV;

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

		// Calculating time in microseconds
		DeltaTimeV = (CurrentFrameTime - LastFrameTime) * 1000000 / QueryFrequency();
		// Division to get milliseconds
		DeltaTimeV /= 1000.f;

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

#include "pch.h"
#include "Time.h"

#include <sstream>
#include <iomanip>


namespace Coil
{
	// initialization of last frame time point with current time resulting in first frame time being time from start of program to first call of Time::Tick()
	std::chrono::time_point<std::chrono::steady_clock> Time::LastFrameTime = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> Time::CurrentFrameTime;

	float32 Time::DeltaTimeV;
	float32 Time::FpsV = 0.f;

	RString<String> Time::TimestampToString(const Timestamp& timestamp)
	{
		std::stringstream ss;
		ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S");
		return ss.str().c_str();
	}

	void Time::Tick()
	{
		// updating current frame time
		CurrentFrameTime = std::chrono::steady_clock::now();

		// using 1/1000 ratio for calculating delta to get precise time reading in ms
		DeltaTimeV = std::chrono::duration<float32, std::ratio<1, 1000>>(CurrentFrameTime - LastFrameTime).count();

		FpsV = 1000.f / DeltaTimeV;

		LastFrameTime = CurrentFrameTime;
	}
}

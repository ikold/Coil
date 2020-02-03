#include "pch.h"
#include "Time.h"

/* TO BE IMPLEMENTED */
namespace Coil
{
	std::chrono::time_point<std::chrono::steady_clock> Time::Last = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> Time::Current;
	
	float Time::DeltaTimeV;
	
	float Time::FpsV = 0.f;
	
	
	void Time::Tick()
	{
		Current = std::chrono::steady_clock::now();
		
		// using 1/1000 ratio to get precise time reading in ms
		DeltaTimeV = (std::chrono::duration<float, std::ratio<1, 1000>>(Current - Last)).count();
		
		FpsV = 1000.f / DeltaTimeV;
		
		Last = Current;
	}
}

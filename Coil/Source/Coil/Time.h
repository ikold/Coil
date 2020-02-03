#pragma once

#include "Coil/Core.h"


namespace Coil
{
	#define Timestamp ::std::time_t
	
	class COIL_API Time
	{
	private:
		Time() {};
	public:
		/*!	Method for geting current timestamp
		*/
		inline static Timestamp Now() { return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());}
		
		static std::string getTimeStr(const Timestamp timestamp)
		{
			std::stringstream ss;
			ss << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S");
			return ss.str();
		}
		
		/*!	Method for calculating frame time
		*/
		static void Tick();
		
		/*!	Get interface for delta time

			@return		float	precalculate delta time
		*/
		inline static float DeltaTime() { return DeltaTimeV; }
		
		/*!	Get interface for fps
		 * 
			@return		float	precalculated fps
		*/
		inline static float Fps() { return FpsV; }
		
		
		
	private:
		static std::chrono::time_point<std::chrono::steady_clock> Last;
		static std::chrono::time_point<std::chrono::steady_clock> Current;
		
		static float DeltaTimeV;
		static float FpsV;
	};
}


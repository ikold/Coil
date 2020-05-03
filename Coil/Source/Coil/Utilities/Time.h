#pragma once

#include "Coil/Core.h"


class TimestampToString;
namespace Coil
{
#define Timestamp ::std::time_t

	/*!
		Static class responsible for all time operations
	 */
	class COIL_API Time
	{
	private:
		Time() {};
	public:
		/*!	Method for geting current timestamp

			@return Timestamp	Timestamp with current date
		*/
		inline static Timestamp Now() { return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); }

		/*!	Formats string from timestamp
			@param[in]	timestamp	Timestamp to be parsed to string

			@return		RString	Formated string
		 */
		static RString<String> TimestampToString(const Timestamp& timestamp);


		/*!	Method for calculating frame time
			Should be called on begging of every frame
		*/
		static void Tick();

		/*!	Get interface for delta time

			@return		float32	precalculate delta time in ms
		*/
		inline static float32 DeltaTime() { return DeltaTimeV; }

		/*!	Get interface for fps

			@return		float32	precalculated fps
		*/
		inline static float32 Fps() { return FpsV; }

	private:
		static std::chrono::time_point<std::chrono::steady_clock> LastFrameTime;		/*!	Coresponds to the time point on the begging of last frame */
		static std::chrono::time_point<std::chrono::steady_clock> CurrentFrameTime;		/*!	Coresponds to the time point on the begging of current frame */

		static float32 DeltaTimeV;	/*!	Precalculated time in ms between frames */
		static float32 FpsV;			/*!	For benchmarking purpose */
	};
}

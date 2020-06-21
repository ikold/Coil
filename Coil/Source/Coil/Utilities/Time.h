#pragma once

#include <ctime>
#include <chrono>


namespace Coil
{
	typedef std::time_t Timestamp;


	/*!
		Static class responsible for all time operations
	 */
	class Time
	{
	private:
		Time() = default;

	public:
		/*!	Method for getting current timestamp

			@return Timestamp	Timestamp with current date
		*/
		static Timestamp Now() { return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); }

		/*!	Formats string from timestamp
			@param[in]	timestamp	Timestamp to be parsed to string

			@return		RString	Formatted string
		 */
		[[nodiscard]] static RString<> TimestampToString(const Timestamp& timestamp);


		/*!	Method for calculating frame time
			Should be called on begging of every frame
		*/
		static void Tick();

		/*!	Get interface for delta time

			@return		float32	precalculated delta time in ms
		*/
		static float32 DeltaTime() { return DeltaTimeV; }

		/*!	Get interface for fps

			@return		float32	precalculated fps
		*/
		static float32 Fps() { return FpsV; }

	private:
		static std::chrono::time_point<std::chrono::steady_clock> LastFrameTime;		/*!	Corresponds to the time point on the begging of last frame */
		static std::chrono::time_point<std::chrono::steady_clock> CurrentFrameTime;		/*!	Corresponds to the time point on the begging of current frame */

		static float32 DeltaTimeV;	/*!	Precalculated time in ms between frames */
		static float32 FpsV;		/*!	For benchmarking purpose */
	};
}

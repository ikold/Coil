#pragma once

#include <ctime>
#include <chrono>


namespace Coil
{
	typedef std::time_t Timestamp;


	/**
	 * @brief Utility class for time related operations
	 *
	 * @todo Parser class for Query results
	 */
	class Time
	{
	public:
		Time() = delete;

		/**
		 * @brief Method for getting current timestamp
		 *
		 * @return	Timestamp with current date
		 */
		static Timestamp Now() { return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); }

		/**
		 * @brief Get steady clock high resolution timestamp
		 *
		 * @return	Local high resolution timestamp in counts
		 *
		 * @note To get time in seconds, divide result by Time::QueryFrequency()
		 * @todo Implement for different platforms
		 */
		static int64 Query();

		/**
		 * @brief Getter for counters frequency
		 *
		 * @return	Counters frequency in counts per second
		 *
		 * @todo Implement for different platforms
		 */
		static int64 QueryFrequency();

		/**
		 * @brief Parses Timestamp to String
		 *
		 * @param[in]	timestamp	Timestamp to be parsed to string
		 * @param[out]	format		Format in which string should be constructed
		 *
		 * @return		Formatted String
		 */
		[[nodiscard]] static RString<> TimestampToString(const Timestamp& timestamp, const RString<>& format = "%Y-%m-%d %H:%M:%S");

		/**
		 * @brief Creates string of current time in specified format
		 *
		 * @param[in]	format		Format in which string should be constructed
		 *
		 * @return		Formatted String
		 */
		static RString<> NowString(const RString<>& format = "%Y-%m-%d %H:%M:%S") { return TimestampToString(Now(), format); }


		/**
		 * @brief Method for calculating frame time
		 *
		 * Should be called on begging of every frame
		 */
		static void Tick();

		/**
		 * @brief Get interface for delta time
		 *
		 * @return	Precalculated delta time in ms
		 */
		static float32 DeltaTime() { return DeltaTimeV; }

		/**
		 * @brief Get interface for fps
		 *
		 * @return	Precalculated fps
		 */
		static float32 Fps() { return FpsV; }

	private:
		static int64 QueryFrequencyImpl();

	private:
		/** Corresponds to the time point on the begging of last frame */
		static int64 LastFrameTime;
		/** Corresponds to the time point on the begging of current frame */
		static int64 CurrentFrameTime;

		/** Precalculated frame time in milliseconds */
		static float32 DeltaTimeV;
		/** Precalculated fps */
		static float32 FpsV;
	};
}

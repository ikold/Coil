#pragma once

#include <ctime>
#include <chrono>


namespace Coil
{
	typedef std::time_t Timestamp;


	/**
	 * @brief Utility class for time related operations
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


		enum class Unit
		{
			Second = 1,
			Millisecond = 1000,
			Microsecond = 1000000,
			Nanosecond = 1000000000
		};


		/**
		 * @brief Converts query result to given unit
		 *
		 * @tparam[in]	T	Returned type
		 */
		template<typename T = int64>
		struct QueryConverter
		{
			/** Target unit divided by it and frequency greatest common divisor */
			const T UnitsInSecondGDC;
			/** Frequency divided by it and target unit greatest common divisor */
			const T FrequencyGDC;

			/**
			 * @param[in]	targetUnit	Unit to convert to
			 * @param[in]	frequency	Frequency of the queries
			 */
			explicit constexpr QueryConverter(Unit targetUnit = Unit::Millisecond, int64 frequency = QueryFrequency())
				: UnitsInSecondGDC(T(targetUnit) / std::gcd(int64(targetUnit), frequency)),
				  FrequencyGDC(T(frequency) / std::gcd(int64(targetUnit), frequency)) {}

			/**
			 * @param[in]	queryResult	
			 *
			 * @return		Converted value in targeted units
			 */
			constexpr T operator()(int64 queryResult) const
			{
				return queryResult * UnitsInSecondGDC / FrequencyGDC;
			}
		};


	private:
		static int64 QueryFrequencyImpl();

	private:
		/** Corresponds to the time point on the begging of last frame */
		inline static int64 LastFrameTime = Query();
		/** Corresponds to the time point on the begging of current frame */
		inline static int64 CurrentFrameTime;

		/** Precalculated frame time in milliseconds */
		inline static float32 DeltaTimeV;
		/** Precalculated fps */
		inline static float32 FpsV;
	};
}

#pragma once

#include <fstream>

#include <thread>
#include <mutex>
#include <atomic>
#include <queue>


#ifdef CL_PROFILE
#define TOKENPASTE_IMPL(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_IMPL(x, y)

/**
 * @brief Creates new high level profiling sessions
 *
 * @param[in]	name		Session name
 * @param[in]	filePath	File path for parsed profiling file
 *
 * @note Should not be called during session as this is undefined behavior
 */
#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filePath)	::Coil::Instrumentor::BeginSession(name, ::Coil::Instrumentor::ProfilingLevel::High, filePath);

 /**
  * @brief Creates new high level profiling sessions
  *
  * @param[in]	name		Session name
  * @param[in]	filePath	File path for parsed profiling file
  */
#define CL_PROFILE_BEGIN_SESSION(name, filePath)		CL_PROFILE_BEGIN_SESSION_HIGH(name, filePath);

  /**
   * @brief Ends currently running profiling session
   */
#define CL_PROFILE_END_SESSION()						::Coil::Instrumentor::EndSession();

   /**
	* @brief Ends currently running profiling session after elapsed time
	*/
#define CL_PROFILE_END_SESSION_TIMER(timeout)			std::thread TOKENPASTE(_endSessionThread_, __LINE__)([&]{\
															Sleep(timeout);\
															CL_PROFILE_END_SESSION()\
														}); TOKENPASTE(_endSessionThread_, __LINE__).detach();


#define CL_PROFILE_REGISTER_TIMER(name)			thread_local int32 TOKENPASTE(_timerID_, __LINE__) = ::Coil::Instrumentor::RegisterTimer(name, GetThreadID());

	/**
	 * @brief Profiling timer implementation
	 *
	 * @param[in]	name	Timer name
	 * @param[in]	level	Profiling level associated with the timer
	 *
	 * @note Timer is RAII controlled (starts at the creation and stops when goes out of scope)
	 * @note Timer is static created on the first pass of the macro, where it registers timer name and retrieves it's ID for future use
	 * @note Each call creates new InstrumentationTimer with ID
	 * @note InstrumentationTimer has minimal overhead when not in profiling session
	 * @note Timer name is reformatted during registration (for function signatures to be more readable)
	 */
	 //#define CL_PROFILE_SCOPE_IMPL(name, level)				CL_PROFILE_REGISTER_TIMER(name)\
 	//															::Coil::InstrumentationTimer TOKENPASTE(_stopWatch_, __LINE__)(TOKENPASTE(_timerID_, __LINE__), level);

	 /**
	  * @brief Creates high level profiling timer
	  *
	  * @param[in]	name	Timer name
	  *
	  * @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
	  */
#define CL_PROFILE_SCOPE_HIGH(name)						CL_PROFILE_REGISTER_TIMER(name)\
															auto TOKENPASTE(_stopWatch_, __LINE__) = ::Coil::Instrumentor::CreateTimer<::Coil::Instrumentor::ProfilingLevel::High>(TOKENPASTE(_timerID_, __LINE__));
	  /**
	   * @brief Creates high level profiling timer
	   *
	   * @param[in]	name	Timer name
	   *
	   * @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
	   */
#define CL_PROFILE_SCOPE(name)							CL_PROFILE_SCOPE_HIGH(name)

	   /**
		* @brief Creates high level profiling timer with function signature as it's name
		*
		* @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
		*/
#define CL_PROFILE_FUNCTION_HIGH()						CL_PROFILE_SCOPE_HIGH(::Coil::Instrumentor::ParseTimerName(__FUNCSIG__))

		/**
		 * @brief Creates high level profiling timer with function signature as it's name
		 *
		 * @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
		 */
#define CL_PROFILE_FUNCTION()							CL_PROFILE_FUNCTION_HIGH()
#else
#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filePath)
#define CL_PROFILE_BEGIN_SESSION(name, filePath)
#define CL_PROFILE_END_SESSION()
#define CL_PROFILE_REGISTER_TIMER(name)
#define CL_PROFILE_SCOPE_IMPL(name, level)
#define CL_PROFILE_SCOPE_HIGH(name)
#define CL_PROFILE_SCOPE(name)
#define CL_PROFILE_FUNCTION_HIGH()
#define CL_PROFILE_FUNCTION()
#endif

#if CL_PROFILE > 1
/**
 * @brief Creates new medium level profiling sessions
 *
 * @param[in]	name		Session name
 * @param[in]	filePath	File path for parsed profiling file
 *
 * @note Should not be called during session as this is undefined behavior
 */
#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filePath)	::Coil::Instrumentor::BeginSession(name, ::Coil::Instrumentor::ProfilingLevel::Medium, filePath);

 /**
  * @brief  @brief Creates medium level profiling timer
  *
  * @param[in]	name	Timer name
  *
  * @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
  */
#define CL_PROFILE_SCOPE_MEDIUM(name)						CL_PROFILE_REGISTER_TIMER(name)\
															auto TOKENPASTE(_stopWatch_, __LINE__) = ::Coil::Instrumentor::CreateTimer<::Coil::Instrumentor::ProfilingLevel::Medium>(TOKENPASTE(_timerID_, __LINE__));

  /**
   * @brief Creates medium level profiling timer with function signature as it's name
   *
   * @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
   */
#define CL_PROFILE_FUNCTION_MEDIUM()					CL_PROFILE_SCOPE_MEDIUM(::Coil::Instrumentor::ParseTimerName(__FUNCSIG__))
#else
#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filePath)
#define CL_PROFILE_SCOPE_MEDIUM(name)
#define CL_PROFILE_FUNCTION_MEDIUM()
#endif

#if CL_PROFILE > 2
/**
 * @brief Creates new low level profiling sessions
 *
 * @param[in]	name		Session name
 * @param[in]	filePath	File path for parsed profiling file
 *
 * @note Should not be called during session as this is undefined behavior
 */
#define CL_PROFILE_BEGIN_SESSION_LOW(name, filePath)	::Coil::Instrumentor::BeginSession(name, ::Coil::Instrumentor::ProfilingLevel::Low, filePath);

 /**
  * @brief  @brief Creates low level profiling timer
  *
  * @param[in]	name	Timer name
  *
  * @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
  */
#define CL_PROFILE_SCOPE_LOW(name)						CL_PROFILE_REGISTER_TIMER(name)\
															auto TOKENPASTE(_stopWatch_, __LINE__) = ::Coil::Instrumentor::CreateTimer<::Coil::Instrumentor::ProfilingLevel::Low>(TOKENPASTE(_timerID_, __LINE__));

  /**
   * @brief Creates low level profiling timer with function signature as it's name
   *
   * @see CL_PROFILE_SCOPE_IMPL(name, level) for more details
   */
#define CL_PROFILE_FUNCTION_LOW()						CL_PROFILE_SCOPE_LOW(::Coil::Instrumentor::ParseTimerName(__FUNCSIG__))
#else
#define CL_PROFILE_BEGIN_SESSION_LOW(name, filePath)
#define CL_PROFILE_SCOPE_LOW(name)
#define CL_PROFILE_FUNCTION_LOW()
#endif


// TODO Move it to its own class
inline int32 GetThreadID()
{
	static std::atomic<int32> thread_counter{};
	const thread_local int32 threadID = thread_counter++;
	return threadID;
}


namespace Coil
{
	/**
	 * @brief Simpleton responsible for gathering, parsing and saving profiling data
	 */
	class Instrumentor
	{
	public:
		/**
		 * @brief Level of profiling
		 */
		enum class ProfilingLevel
		{
			None,
			High,
			Medium,
			Low
		};

		/**
		 * @brief Data structure for storing Instrumentation session details
		*/
		struct Session
		{
			/** Name of the session */
			RString<> Name;
			/** File path for parsed json file */
			RString<> OutputFilePath;
			/** File path for binary dump */
			RString<> BinaryFilePath;
			/** Session level at witch profiling result is accepted */
			ProfilingLevel Level;

			int64 StartTime;
			/** If delete binary after parsing to json is done */
			bool DeleteProfilingBinary = true;
		};

		static RString<> ProfilingLevelToString(ProfilingLevel level)
		{
			switch (level)
			{
			case ProfilingLevel::None: return "None";
			case ProfilingLevel::High: return "High";
			case ProfilingLevel::Medium: return "Medium";
			case ProfilingLevel::Low: return "Low";
			}

			return "";
		}

	protected:
		struct TimerData
		{
			int32 ID;
			int32 ThreadID;
			RString<> Name;
		};

		struct WatchData
		{
			int32 Start;
			int32 End;
			int32 TimerID;
		};

		template<ProfilingLevel TProfileLevel>
		class StopWatch
		{
		public:
			StopWatch()
				: Data{ 0, 0, 0 }
			{}

			explicit StopWatch(int32 timerIndex)
				: Data{ static_cast<int32>(Time::Query() - CurrentSession.StartTime), 1, timerIndex }
			{}

			~StopWatch()
			{
				if (Data.End)
				{
					Data.End = static_cast<int32>(Time::Query() - CurrentSession.StartTime);
					SaveWatchData(Data);
				}
			}

			StopWatch(const StopWatch&) = delete;
			StopWatch(StopWatch&&) = delete;
			StopWatch& operator=(const StopWatch&) = delete;
			StopWatch& operator=(const StopWatch&&) = delete;

		private:
			WatchData Data;
		};

	public:
		/**
		 * @brief Starts instrumentation session
		 *
		 * @param[in]	name		Session name
		 * @param[in]	level		Level of profiling
		 * @param[in]	filePath	File path for parsed profiling file
		 *
		 * @note If called during another session, results are integrated to new session, old session is not saved to file
		 */
		static void BeginSession(const RString<>& name, ProfilingLevel level, const RString<>& filePath = "Profiling/Profiling.json")
		{
			std::lock_guard beginSessionLock(ProfilesMutex);

			if (InSession())
			{
				CurrentSession.StartTime = Time::Query();
				CurrentSession.Name = name;
				CurrentSession.Level = level;
				CurrentSession.OutputFilePath = filePath;
				return;
			}

			auto timestamp = Time::NowString();
			timestamp->Replace(':', '_');

			ResetData();

			CurrentSession = Session{ name, filePath, PString("Profiling/%R.prof", &timestamp), level, Time::Query() };
		}

		static void SaveWatchData(const WatchData& data)
		{
			if (DataIndex >= DataSizeThreshold)
				SafeSaveWatchData(data);
			else
				Data[DataIndex++] = data;
		}


		static void SafeSaveWatchData(const WatchData& data)
		{
			static std::mutex resizeMutex;
			auto resizeLock = std::unique_lock(resizeMutex, std::defer_lock);

			if (!resizeLock.try_lock())
			{
				const int64 index = DataIndex++;

				//if current index is outside the array wait for the resize to complete by waiting for the mutex to unlock
				if (index >= DataSize)
					resizeLock.lock();

				Data[index] = data;
			}

			ResizeData();
		}

		static void ResizeData()
		{
			CL_PROFILE_FUNCTION_LOW()

				WatchData* oldData = Data;
			Data = new WatchData[static_cast<int64>(DataSize) * 2]();

			WatchData* iterator = oldData + DataIndex;

			DataSize *= 2;
			DataSizeThreshold = DataSize - 1000;

			// setting iterator on the last non zero watch in the old data set
			while (iterator->Start == 0 && --iterator >= oldData)
			{
			}

			memcpy(Data, oldData, (++iterator - oldData) * sizeof WatchData);

			delete[] oldData;
		}

		template<ProfilingLevel TProfileLevel>
		static [[nodiscard]] StopWatch<TProfileLevel> CreateTimer(int32 timerID)
		{
			if (CurrentSession.Level >= TProfileLevel)
				return StopWatch<TProfileLevel>(timerID);

			return StopWatch<TProfileLevel>();
		}

		/**
		 * @brief Ends any active session
		 *
		 * @bug Properly implement parsing on the second thread. Currently thread crashes if application is closed during parsing.
		 */
		static void EndSession()
		{
			std::lock_guard endSessionLock(ProfilesMutex);

			if (!InSession())
				return;

			SaveProfiling(CurrentSession.BinaryFilePath);

			// Using separate thread for parsing
			std::thread parserThread(ParseDumpFileToJson, CurrentSession.BinaryFilePath, CurrentSession.OutputFilePath, CurrentSession.DeleteProfilingBinary);
			parserThread.detach();

			CurrentSession = Session();
		}

		/**
		 * @brief Caches timer name
		 *
		 * @param[in]	name	Name to be cached
		 *
		 * @return		ID of the cached timer name
		 *
		 * @note Name is reformatted for better readability
		 *
		 * @todo Check for existing names and return their ID instead of creating duplicate
		 */
		static int32 RegisterTimer(const RString<>& name, int32 threadID)
		{
			std::lock_guard registerNameLock(ProfilesMutex);

			const int32 id = static_cast<int32>(RegisteredTimers.size());

			RegisteredTimers.push_back({ id, threadID, name });

			return id;
		}

		static RString<> ParseTimerName(const char* name)
		{
			RString rName(name);

			rName->Replace('"', '\'');

			rName->Remove("void ");
			rName->Remove("__cdecl ");
			rName->Remove("Coil::");
			rName->Remove("struct ");
			rName->Remove("class ");

			rName->Replace("__int64", "int64");

			rName->Replace("glm::vec<4,float,0>", "vec4");
			rName->Replace("glm::vec<3,float,0>", "vec3");
			rName->Replace("glm::vec<2,float,0>", "vec2");

			rName->Replace("std::shared_ptr", "Ref");
			rName->Replace("RString<String>", "RString");

			rName->Replace(",", ", ");
			rName->Replace("  ", " ");

			return rName;
		}

		/**
		 * @brief Checks if there is ongoing session
		 *
		 * @param[in]	profile		Level of session
		 *
		 * @return		True if in session of equal or greater level
		 */
		static [[nodiscard]] bool InSession(ProfilingLevel profile = ProfilingLevel::High)
		{
			return CurrentSession.Level >= profile;
		}

		/**
		 * @brief Dumps profiling binary to selected path
		 *
		 * @param[in]	filePath
		 *
		 * @note Instrumentor::TimerNames are saved in the same file as binary
		 * @note If there is no profiling data, cached names are still saved to file
		 * @note All of the data in Instrumentor::Profiles is cleared upon completion
		 * @note File depends on ProfileResult struct memory layout
		 *
		 * @note\n
		 * File layout:\n
		 * 8 Bytes - Frequency of watches data\n
		 * 8 Bytes - Number of watches\n
		 * 4 Bytes - Number of registered timers\n
		 * 4 Bytes - Offset to profiling data\n
		 * [Number of registered timers] x\n
		 * |  4 Bytes - Timer ID
		 * |  4 Bytes - Timer thread ID
		 * |  4 Bytes - Timer name length\n
		 * |  [Timer name length] Bytes - Timer name as uncompressed non-zero terminated string\n
		 * [number of watches] x [WatchData struct size] Bytes - Raw binary of profiling data
		 *
		 * @todo Ensure path creation if it does not exist
		 */
		static void SaveProfiling(const RString<>& filePath)
		{
			std::ofstream outputStream(filePath->CString(), std::ios::out | std::ios::binary);

			int64 frequency = Time::QueryFrequency();
			outputStream.write(reinterpret_cast<char8*>(&frequency), sizeof int64);

			outputStream.write(reinterpret_cast<char8*>(&DataIndex), sizeof int64);

			int64 numberOfNames = RegisteredTimers.size();
			outputStream.write(reinterpret_cast<char8*>(&numberOfNames), sizeof int64); // Higher 4 bytes are overrode by dataOffset in later code 

			for (const auto& timer : RegisteredTimers)
			{
				outputStream.write(reinterpret_cast<const char8*>(&timer.ID), sizeof int32);
				outputStream.write(reinterpret_cast<const char8*>(&timer.ThreadID), sizeof int32);

				int32 nameLength = timer.Name->GetLength();
				outputStream.write(reinterpret_cast<char8*>(&nameLength), sizeof int32);
				outputStream.write(timer.Name->CString(), nameLength);
			}

			int32 dataOffset = static_cast<int32>(outputStream.tellp());

			// Reversing profile due to chrome://tracing issue where events that starts at the same time are draw incorrectly if first event ended before the second

			outputStream.write(reinterpret_cast<char8*>(Data), DataIndex * sizeof WatchData);

			// Overriding unused higher bytes of number of registered timers with data offset
			outputStream.seekp(20, std::ios_base::beg);
			outputStream.write(reinterpret_cast<char8*>(&dataOffset), sizeof int32);
		}

		/**
		 * @brief Parses profiling results to json file
		 *
		 * @param[in]	sourcePath		File path to profiling binary
		 * @param[in]	destPath		File to save parsed data (should have .json extension)
		 * @param[in]	deleteSource	If to delete profiling binary upon completion
		 *
		 * @note json file is in format readable by chrome://tracing
		 *
		 * @todo Ensure path creation if it does not exist
		 */
		static void ParseDumpFileToJson(const RString<>& sourcePath, const RString<>& destPath, bool deleteSource = false)
		{
			const int64 start = Time::Query();

			RString parsingProgression = PString("Parsing profiling file to json %4d%% done\nSource:      %R %{#deleted}s\nDestination: %R", 0, &sourcePath, "", &destPath);
			Logger::Trace(parsingProgression);

			std::ifstream inputStream(sourcePath->CString(), std::ios::binary | std::ios::in);
			std::vector<TimerData> registeredTimers;

			// Used later in PString to reserve enough space for timer names
			int32 longestNameLength = 0;

			int64 frequency = Time::QueryFrequency();
			inputStream.read(reinterpret_cast<char8*>(&frequency), sizeof int64);

			int64 numberOfProfiles;
			inputStream.read(reinterpret_cast<char8*>(&numberOfProfiles), sizeof int64);

			int32 numberOfRegisteredTimers;
			inputStream.read(reinterpret_cast<char8*>(&numberOfRegisteredTimers), sizeof int32);

			// Skipping offset to profiling data
			inputStream.ignore(sizeof int32);

			registeredTimers.reserve(numberOfRegisteredTimers);

			// Retrieving timer names
			for (int32 i = 0; i < numberOfRegisteredTimers; ++i)
			{
				int32 timerID{};
				int32 threadID{};
				int32 nameLength{};
				inputStream.read(reinterpret_cast<char8*>(&timerID), sizeof int32);
				inputStream.read(reinterpret_cast<char8*>(&threadID), sizeof int32);
				inputStream.read(reinterpret_cast<char8*>(&nameLength), sizeof int32);

				auto* rawName = new char8[static_cast<size_t>(nameLength) + 1];
				rawName[nameLength] = '\0';

				inputStream.read(rawName, nameLength);

				registeredTimers.push_back({ timerID, threadID, String(&rawName, nameLength) });

				if (registeredTimers.back().Name->GetLength() > longestNameLength)
					longestNameLength = registeredTimers.back().Name->GetLength();
			}

			// Creating output file
			std::ofstream outputStream(destPath->CString());

			// Using nanosecond for better precision
			outputStream << R"({"otherData":{},"displayTimeUnit":"ns","traceEvents":[)";

			int64 processedProfiles = 0;

			WatchData watch{};
			Time::QueryConverter queryToNanoseconds(Time::Unit::Nanosecond, frequency);

			// Profiling entry template
			// chrome tracing for some reason treats 1.000 as a 1000 ns even though it should use nanoseconds
			PString profilingEntry(R"({"ph":"X","pid":0,"tid":%d,"name":"%S","cat":"function","ts":%24{.3}d,"dur":%{.3}d},)", 0, 0, longestNameLength, 0, 0);

			while (++processedProfiles <= numberOfProfiles)
			{
				inputStream.read(reinterpret_cast<char8*>(&watch), sizeof WatchData);

				// Filling profiling entry template
				profilingEntry.SetIndex(0, registeredTimers.at(watch.TimerID).ThreadID);
				profilingEntry.SetIndex(1, registeredTimers.at(watch.TimerID).Name->CString());
				profilingEntry.SetIndex(2, queryToNanoseconds(watch.Start));
				profilingEntry.SetIndex(3, queryToNanoseconds(watch.End - watch.Start));

				outputStream << profilingEntry.ToString().CString();

				// Updating progress information
				parsingProgression->SetIndex(0, processedProfiles * 100 / numberOfProfiles);
			}

			outputStream << R"({}]})";

			inputStream.close();

			if (deleteSource)
			{
				remove(sourcePath->CString());
				parsingProgression->Set("deleted", "[Deleted]");
			}

			Logger::Trace("Parsing profiles took %d ms", Time::QueryConverter(Time::Unit::Millisecond)(Time::Query() - start));
		}

		struct Statistics
		{
			uint32 NumberOfProfiles{};
			uint64 MemoryUsed{};
			uint64 MemoryReserved{};
			int64 ElapsedTime{};
			ProfilingLevel ProfilingLevel{};
		};

		static Statistics GetStatistics()
		{
			std::lock_guard statisticsLock(ProfilesMutex);

			if (!InSession())
				return Statistics{};

			const Statistics stats = {
				DataIndex,
				DataIndex * sizeof WatchData,
				DataSize * sizeof WatchData,
				Time::Query() - CurrentSession.StartTime,
				CurrentSession.Level
			};

			return stats;
		}

	private:
		static void ResetData()
		{
			WatchData* oldData = Data;
			auto* newData = new WatchData[512 * 1024 / 12]();
			DataIndex = 0;
			Data = newData;
			DataSize = 512 * 1024 / 12;
			DataSizeThreshold = DataSize - 1200;

			delete[] oldData;
		}

	private:
		static inline Session CurrentSession;
		/** Cached names of the timers */
		static inline std::vector<TimerData> RegisteredTimers;
		static inline std::mutex ProfilesMutex;
		static inline std::atomic<int64> DataIndex = -1;
		static inline WatchData* Data = nullptr;
		static inline int32 DataSize = 0;
		static inline int32 DataSizeThreshold = 0;
	};
}

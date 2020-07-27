#pragma once

#include <fstream>

#include <thread>
#include <mutex>
#include <atomic>


// TODO move it to it's own class
inline uint32 GetThreadID()
{
	static std::atomic<uint32> thread_counter{};
	thread_local uint32 threadID = thread_counter++;
	return threadID;
}


namespace Coil
{
	/**
	 * @brief Data structure for storing profiling results
	 */
	struct ProfileResult
	{
		int64 Start, End;
		uint32 ThreadID;
		uint32 NameID;
	};


	/**
	 * @brief Level of profiling
	 */
	enum class SessionProfile
	{
		Low,
		Medium,
		High
	};


	/**
	 * @brief Data structure for storing Instrumentation session details
	 */
	struct InstrumentationSession
	{
		/** Name of the session */
		RString<> Name;
		/** File path for parsed json file */
		RString<> OutputFilePath;
		/** File path for binary dump */
		RString<> BinaryFilePath;
		/** Session level at witch profiling result is accepted */
		SessionProfile Profile;
		/** If delete binary after parsing to json is done */
		bool DeleteProfilingBinary = true;
	};


	/**
	 * @brief Simpleton responsible for gathering, parsing and saving profiling data.
	 */
	class Instrumentor
	{
	public:
		Instrumentor()
			: CurrentSession(nullptr) {}

		/**
		 * @brief Starts instrumentation session.
		 *
		 * @note If called during another session, results are integrated to new session, old session is not saved to file
		 */
		void BeginSession(const RString<>& name, SessionProfile profile, const RString<>& filePath = "Profiling/Profiling.json")
		{
			std::lock_guard beginSessionLock(ProfilesMutex);

			if (CurrentSession != nullptr)
			{
				CurrentSession->Name           = name;
				CurrentSession->Profile        = profile;
				CurrentSession->OutputFilePath = filePath;
				return;
			}

			Profiles.reserve(0x1000);

			auto timestamp = Time::NowString();
			timestamp->Replace(':', '_');

			CurrentSession = new InstrumentationSession{ name, filePath, PString("Profiling/%R.prof", &timestamp), profile };
		}

		/**
		 * @brief Ends any active session
		 *
		 * @bug Properly implement parsing on the second thread. Currently thread crashes if application is closed during parsing.
		 */
		void EndSession()
		{
			std::lock_guard endLock(ProfilesMutex);

			if (CurrentSession == nullptr)
				return;

			// Setting CurrentSession to nullptr so Instrumentor::InSession(SessionProfile) returns fall and no more new data is added during saving to file
			InstrumentationSession* session = CurrentSession;
			CurrentSession                  = nullptr;

			SaveProfiling(session->BinaryFilePath);

			// Using separate thread for parsing
			std::thread parserThread(ParseDumpFileToJson, session->BinaryFilePath, session->OutputFilePath, session->DeleteProfilingBinary);
			parserThread.detach();

			delete session;
		}

		/**
		 * @brief Caches timer name
		 *
		 * @param[in]	name	Name to be cached
		 *
		 * @return				ID of the cached timer name
		 *
		 * @note Name is reformatted for better readability
		 */
		int32 RegisterName(const char* name)
		{
			TimerNames.emplace_back(name);

			TimerNames.back()->Replace('"', '\'');

			TimerNames.back()->Remove("void ");
			TimerNames.back()->Remove("void");
			TimerNames.back()->Remove("__cdecl ");
			TimerNames.back()->Remove("Coil::");
			TimerNames.back()->Remove("struct ");
			TimerNames.back()->Remove("class ");

			TimerNames.back()->Replace("__int64", "int64");

			TimerNames.back()->Replace("glm::vec<4,float,0>", "vec4");
			TimerNames.back()->Replace("glm::vec<3,float,0>", "vec3");
			TimerNames.back()->Replace("glm::vec<2,float,0>", "vec2");

			TimerNames.back()->Replace("std::shared_ptr", "Ref");
			TimerNames.back()->Replace("RString<String>", "RString");

			TimerNames.back()->Replace(",", ", ");
			TimerNames.back()->Replace("  ", " ");

			return TimerNames.size() - 1;
		}

		/**
		 * @brief Creates profile if currently in session
		 *
		 * @param[in]	start		Begging timestamp in counts
		 * @param[in]	end			Ending timestamp in counts
		 * @param[in]	threadID
		 * @param[in]	nameID		ID of the cached name
		 */
		void CreateProfile(int64 start, int64 end, uint32 threadID, uint32 nameID)
		{
			std::lock_guard writeLock(ProfilesMutex);

			if (!CurrentSession)
				return;

			Profiles.emplace_back(ProfileResult{ start, end, threadID, nameID });
		}

		/**
		 * @brief Checks if there is ongoing session
		 *
		 * @param[in]	profile		Level of session
		 *
		 * @return					True if in session of equal or greater level
		 */
		[[nodiscard]] bool InSession(SessionProfile profile = SessionProfile::High) const
		{
			return CurrentSession != nullptr && CurrentSession->Profile <= profile;
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
		 * 8 Bytes - Number of profiles\n
		 * 4 Bytes - Number of timer names\n
		 * 4 Bytes - Offset to profiling data\n
		 * [Number of timers names] x\n
		 * |  4 Bytes - Timer name length\n
		 * |  [Timer name length] Bytes - Timer name as uncompressed non-zero terminated string\n
		 * [number of profiles] x [ProfileResult struct size] Bytes - Raw binary of profiling data
		 *
		 * @todo Ensure path creation if does not exist
		 * @todo Save Time::countersFrequency() to file
		 */
		void SaveProfiling(const RString<>& filePath)
		{
			std::ofstream outputStream(filePath->CString(), std::ios::out | std::ios::binary);

			int64 numberOfProfiles = Profiles.size();
			outputStream.write(reinterpret_cast<char8*>(&numberOfProfiles), sizeof int64);

			int64 numberOfNames = TimerNames.size();
			outputStream.write(reinterpret_cast<char8*>(&numberOfNames), sizeof int64); // Higher 4 bytes are overrode by dataOffset in later code 

			for (const auto& name : TimerNames)
			{
				int32 nameLength = name->GetLength();
				outputStream.write(reinterpret_cast<char8*>(&nameLength), sizeof int32);
				outputStream.write(name->CString(), nameLength);
			}

			// Returns if there is no profiling data to be saved
			if (!numberOfProfiles)
				return;

			int32 dataOffset = outputStream.tellp();

			outputStream.write(reinterpret_cast<char8*>(&Profiles[0]), numberOfProfiles * sizeof ProfileResult);

			// Overriding unused higher bytes of numberOfNames
			outputStream.seekp(12, std::ios_base::beg);
			outputStream.write(reinterpret_cast<char8*>(&dataOffset), sizeof int32);


			Profiles.clear();
			Profiles.shrink_to_fit();
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
		 * @todo Ensure path creation if does not exist
		 */
		static void ParseDumpFileToJson(const RString<>& sourcePath, const RString<>& destPath, bool deleteSource = false)
		{
			const int64 start = Time::Query();

			RString parsingProgression = PString("Parsing profiling file to json %4d%% done\nSource:      %R %s\nDestination: %R", 0, &sourcePath, "", &destPath);
			Logger::Trace(parsingProgression);

			std::ifstream inputStream(sourcePath->CString(), std::ios::binary | std::ios::in);
			std::vector<RString<>> fileTimerNames;

			// Used later in PString to reserve enough space for timer names
			int32 longestNameLength = 0;

			int64 numberOfProfiles;
			inputStream.read(reinterpret_cast<char8*>(&numberOfProfiles), sizeof int64);

			int32 numberOfNames;
			inputStream.read(reinterpret_cast<char8*>(&numberOfNames), sizeof int32);

			// Skipping offset to profiling data
			inputStream.ignore(sizeof int32);

			fileTimerNames.reserve(numberOfNames);

			// Retrieving timer names
			for (int32 i = 0; i < numberOfNames; ++i)
			{
				int32 nameLength;
				inputStream.read(reinterpret_cast<char8*>(&nameLength), sizeof int32);

				auto* rawName       = new char8[static_cast<size_t>(nameLength) + 1];
				rawName[nameLength] = '\0';

				inputStream.read(rawName, nameLength);

				fileTimerNames.emplace_back(String(&rawName, nameLength));

				if (fileTimerNames.back()->GetLength() > longestNameLength)
					longestNameLength = fileTimerNames.back()->GetLength();
			}

			// Creating output file
			std::ofstream outputStream(destPath->CString());

			// Using nanosecond for better precision
			outputStream << R"({"otherData":{},"displayTimeUnit":"ns","traceEvents":[)";

			int64 processedProfiles = 0;

			ProfileResult profile{};

			// Used to convert timer counter times to nanoseconds
			int64 nanosecond        = 1000000000;
			int64 countersFrequency = Time::QueryFrequency();

			// To prevent arithmetic overflow, nanosecond and countersFrequency are divided by the greatest common divisor
			int64 gcd = std::gcd(nanosecond, countersFrequency);

			countersFrequency /= gcd;
			nanosecond /= gcd;

			// Profiling entry template
			PString profilingEntry(R"({"ph":"X","pid":0,"tid":%d,"name":"%S","cat":"function","ts":%24{.3}d,"dur":%{.3}d},)", 0, "", longestNameLength, 0, 0, 0, 0);

			while (++processedProfiles <= numberOfProfiles)
			{
				inputStream.read(reinterpret_cast<char8*>(&profile), sizeof ProfileResult);

				// Filling profiling entry template
				profilingEntry.Set(0, profile.ThreadID);
				profilingEntry.Set(1, fileTimerNames.at(profile.NameID)->CString());
				profilingEntry.Set(2, profile.Start * nanosecond / countersFrequency);
				profilingEntry.Set(3, (profile.End - profile.Start) * nanosecond / countersFrequency);

				outputStream << profilingEntry.ToString().CString();

				// Updating progress information
				parsingProgression->Set(0, processedProfiles * 100 / numberOfProfiles);
			}

			outputStream << R"({}]})";

			inputStream.close();

			if (deleteSource)
			{
				remove(sourcePath->CString());
				parsingProgression->Set(2, "[Deleted]");
			}

			Logger::Trace(PString("Parsing profiles took %d ms", (Time::Query() - start) * 1000 / Time::QueryFrequency()));
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* CurrentSession;
		/** Cached names of the timers */
		std::vector<RString<>> TimerNames{};
		std::vector<ProfileResult> Profiles{};
		std::mutex ProfilesMutex;
	};


	/**
	 * @brief Timer controlled by it's lifetime
	 *
	 * @note Timer uses counts for smaller overhead, see Time::Query() for more details
	 */
	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(uint32 nameID, SessionProfile sessionProfile)
		{
			// If not in session no additional instructions are executed to minimize overhead of inactive timers
			Stopped = !Instrumentor::Get().InSession(sessionProfile);

			if (Stopped)
				return;

			NameID         = nameID;
			StartTimepoint = Time::Query();
		}

		~InstrumentationTimer()
		{
			if (!Stopped)
				Stop();
		}

		/**
		 * @brief Creates profiling data in Instrumentor
		 */
		void Stop()
		{
			Instrumentor::Get().CreateProfile(StartTimepoint, Time::Query(), GetThreadID(), NameID);

			Stopped = true;
		}

	private:
		/** ID corresponding to cached timer name */
		uint32 NameID{};
		int64 StartTimepoint{};
		bool Stopped;
	};
}


#ifdef CL_PROFILE
	#define TOKENPASTE_IMPL(x, y) x ## y
	#define TOKENPASTE(x, y) TOKENPASTE_IMPL(x, y)

	#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filePath)	::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::High, filePath);

	#define CL_PROFILE_BEGIN_SESSION(name, filePath)		CL_PROFILE_BEGIN_SESSION_HIGH(name, filePath);

	#define CL_PROFILE_END_SESSION()						::Coil::Instrumentor::Get().EndSession();

	#define CL_PROFILE_SCOPE_IMPL(name, level)				static int32 TOKENPASTE(_nameID_, __LINE__) = Coil::Instrumentor::Get().RegisterName(name);\
																			::Coil::InstrumentationTimer TOKENPASTE(_timer_, __LINE__)(TOKENPASTE(_nameID_, __LINE__), level);

	#define CL_PROFILE_SCOPE_HIGH(name)						CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::High)

	#define CL_PROFILE_SCOPE(name)							CL_PROFILE_SCOPE_HIGH(name)

	#define CL_PROFILE_FUNCTION_HIGH()						CL_PROFILE_SCOPE_HIGH(__FUNCSIG__)

	#define CL_PROFILE_FUNCTION()							CL_PROFILE_FUNCTION_HIGH()
	#else
	#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filePath)
	#define CL_PROFILE_BEGIN_SESSION(name, filePath)
	#define CL_PROFILE_END_SESSION()
	#define CL_PROFILE_SCOPE_IMPL(name, level)
	#define CL_PROFILE_SCOPE_HIGH(name)
	#define CL_PROFILE_SCOPE(name)
	#define CL_PROFILE_FUNCTION_HIGH()
	#define CL_PROFILE_FUNCTION()
#endif

#if CL_PROFILE > 1
	#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filePath)	::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::Medium, filePath);

	#define CL_PROFILE_SCOPE_MEDIUM(name)					CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::Medium)

	#define CL_PROFILE_FUNCTION_MEDIUM()					CL_PROFILE_SCOPE_MEDIUM(__FUNCSIG__)
#else
	#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filePath)
	#define CL_PROFILE_SCOPE_MEDIUM(name)
	#define CL_PROFILE_FUNCTION_MEDIUM()
#endif

#if CL_PROFILE > 2
	#define CL_PROFILE_BEGIN_SESSION_LOW(name, filePath)	::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::Low, filePath);

	#define CL_PROFILE_SCOPE_LOW(name)						CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::Low)

	#define CL_PROFILE_FUNCTION_LOW()						CL_PROFILE_SCOPE_LOW(__FUNCSIG__)
#else
	#define CL_PROFILE_BEGIN_SESSION_LOW(name, filePath)
	#define CL_PROFILE_SCOPE_LOW(name)
	#define CL_PROFILE_FUNCTION_LOW()
#endif

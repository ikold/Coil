#pragma once

#include <chrono>
#include <fstream>

#include <thread>
#include <future>
#include <iostream>


namespace Coil
{
	struct ProfileResult
	{
		int64 Start, End;
		uint32 ThreadID;
		uint32 NameID;
	};


	enum class SessionProfile
	{
		Low,
		Medium,
		High
	};


	struct InstrumentationSession
	{
		RString<> Name;
		RString<> OutputFilepath;
		RString<> BinaryFilepath;
		SessionProfile Profile;
		bool DeleteProfilingBinary = true;
		int64 Created              = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	};


	class Instrumentor
	{
	public:
		Instrumentor()
			: CurrentSession(nullptr)
		{}

		void BeginSession(const RString<>& name, SessionProfile profile, const RString<>& filepath = "Profiling/Profiling.json")
		{
			Profiles.reserve(0x1000);

			auto timestamp = Time::NowString();
			timestamp->Replace(':', '_');

			CurrentSession = new InstrumentationSession{ name, filepath, PString("Profiling/%R.prof", &timestamp), profile };
		}

		void EndSession()
		{
			if (CurrentSession == nullptr)
				return;
			
			SaveProfiling();

			std::thread parserThread(ParseDumpFileToJson, CurrentSession->BinaryFilepath, CurrentSession->OutputFilepath, CurrentSession->DeleteProfilingBinary);
			parserThread.detach();

			delete CurrentSession;
			CurrentSession = nullptr;
		}

		int32 RegisterName(const char* name)
		{
			TimerNames.emplace_back(name);
			TimerNames.back()->Replace('"', '\'');
			TimerNames.back()->Remove("void __cdecl ");
			TimerNames.back()->Remove("__cdecl ");

			return TimerNames.size() - 1;
		}

		void CreateProfile(int64 start, int64 end, uint32 threadID, uint32 nameID)
		{
			start -= CurrentSession->Created;
			end -= CurrentSession->Created;

			Profiles.push_back({ start, end, threadID, nameID });
		}

		[[nodiscard]] bool InSession(SessionProfile profile = SessionProfile::High) const
		{
			return CurrentSession != nullptr && CurrentSession->Profile <= profile;
		}

		void SaveProfiling()
		{
			// TODO Ensure path creation if doesn't exist
			std::ofstream outputStream(CurrentSession->BinaryFilepath->CString(), std::ios::out | std::ios::binary);
			{
				const auto startTimepoint = std::chrono::high_resolution_clock::now();

				int64 numberOfProfiles = Profiles.size();
				outputStream.write(reinterpret_cast<char8*>(&numberOfProfiles), sizeof int64);

				int64 numberOfNames = TimerNames.size();
				outputStream.write(reinterpret_cast<char8*>(&numberOfNames), sizeof int64);

				for (const auto& name : TimerNames)
				{
					int32 nameLength = name->GetLength();
					outputStream.write(reinterpret_cast<char8*>(&nameLength), sizeof int32);
					outputStream.write(name->CString(), name->GetLength());
				}

				const auto endTimepoint = std::chrono::high_resolution_clock::now();

				const int64 start = std::chrono::time_point_cast<std::chrono::milliseconds>(startTimepoint).time_since_epoch().count();
				const int64 end   = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

				Logger::Trace(PString("saving names: %d ms", static_cast<int32>(end - start)));
			}


			{
				const auto startTimepoint = std::chrono::high_resolution_clock::now();

				int32 dataOffset = outputStream.tellp();

				outputStream.write(reinterpret_cast<char8*>(&Profiles[0]), Profiles.size() * sizeof ProfileResult);

				outputStream.seekp(12, std::ios_base::beg);
				outputStream.write(reinterpret_cast<char8*>(&dataOffset), sizeof int32);


				Profiles.clear();
				Profiles.shrink_to_fit();

				const auto endTimepoint = std::chrono::high_resolution_clock::now();

				const int64 start = std::chrono::time_point_cast<std::chrono::milliseconds>(startTimepoint).time_since_epoch().count();
				const int64 end   = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

				Logger::Trace(PString("saving profiles: %d ms", static_cast<int32>(end - start)));
			}
		}

		static void ParseDumpFileToJson(const RString<>& sourcePath, const RString<>& destPath, bool deleteSource = false)
		{
			const auto startTimepoint = std::chrono::high_resolution_clock::now();

			// TODO Ensure path creation if doesn't exist
			RString parsingProgression = PString("Parsing profiling file to json %4d%% done\nSource:      %R %s\nDestination: %R", 0, &sourcePath, "", &destPath);
			Logger::Trace(parsingProgression);

			std::ifstream inputStream(sourcePath->CString(), std::ios::binary | std::ios::in);
			std::vector<RString<>> fileTimerNames;

			int64 numberOfProfiles;
			inputStream.read(reinterpret_cast<char8*>(&numberOfProfiles), sizeof int64);

			int32 numberOfNames;
			inputStream.read(reinterpret_cast<char8*>(&numberOfNames), sizeof int32);

			inputStream.ignore(sizeof int32); // skipping size in bytes

			fileTimerNames.reserve(numberOfNames);

			int32 longestNameLength = 0;

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

			std::ofstream outputStream(destPath->CString());

			outputStream << R"({"otherData":{},"displayTimeUnit":"ns","traceEvents":[)";

			int64 processedProfiles = 0;

			ProfileResult profile{};

			PString profilingEntry(R"({"ph":"X","pid":0,"tid":%d,"name":"%S","cat":"function","ts":%24{.3}d,"dur":%{.3}d},)", 0, "", longestNameLength, 0, 0, 0, 0);

			while (++processedProfiles <= numberOfProfiles)
			{
				inputStream.read(reinterpret_cast<char8*>(&profile), sizeof ProfileResult);

				profilingEntry.Set(0, static_cast<int32>(profile.ThreadID));
				profilingEntry.Set(1, fileTimerNames.at(profile.NameID)->CString());
				profilingEntry.Set(2, profile.Start);
				profilingEntry.Set(3, profile.End - profile.Start);

				outputStream << profilingEntry.ToString().CString();

				parsingProgression->Set(0, static_cast<int32>(processedProfiles * 100 / numberOfProfiles));
			}

			outputStream << R"({}]})";

			inputStream.close();
			if (deleteSource)
			{
				remove(sourcePath->CString());
				parsingProgression->Set(2, "[Deleted]");
			}


			const auto endTimepoint = std::chrono::high_resolution_clock::now();

			const int64 start = std::chrono::time_point_cast<std::chrono::milliseconds>(startTimepoint).time_since_epoch().count();
			const int64 end   = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

			Logger::Trace(PString("parsing profiles: %d ms", static_cast<int32>(end - start)));
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* CurrentSession;
		std::vector<RString<>> TimerNames{};
		std::vector<ProfileResult> Profiles{};
	};


	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(uint32 nameID, SessionProfile sessionProfile)
			: NameID(nameID)
		{
			Stopped = !Instrumentor::Get().InSession(sessionProfile);

			if (Stopped)
				return;

			StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!Stopped && Instrumentor::Get().InSession())
				Stop();
		}

		void Stop()
		{
			const auto endTimepoint = std::chrono::high_resolution_clock::now();

			// TODO Get session start

			const int64 start = std::chrono::time_point_cast<std::chrono::nanoseconds>(StartTimepoint).time_since_epoch().count();
			const int64 end   = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint).time_since_epoch().count();

			const uint32 threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().CreateProfile(start, end, threadID, NameID);

			Stopped = true;
		}

	private:
		uint32 NameID;
		std::chrono::time_point<std::chrono::high_resolution_clock> StartTimepoint;
		bool Stopped;
	};
}


#ifdef CL_PROFILE
	#define TOKENPASTE_IMPL(x, y) x ## y
	#define TOKENPASTE(x, y) TOKENPASTE_IMPL(x, y)

	#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filepath)	::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::High, filepath);

	#define CL_PROFILE_BEGIN_SESSION(name, filepath)		CL_PROFILE_BEGIN_SESSION_HIGH(name, filepath);

	#define CL_PROFILE_END_SESSION()						::Coil::Instrumentor::Get().EndSession();

	#define CL_PROFILE_SCOPE_IMPL(name, level)				static int32 TOKENPASTE(nameID, __LINE__) = Coil::Instrumentor::Get().RegisterName(name);\
																		::Coil::InstrumentationTimer TOKENPASTE(timer, __LINE__)(TOKENPASTE(nameID, __LINE__), level);

	#define CL_PROFILE_SCOPE_HIGH(name)						CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::High)

	#define CL_PROFILE_SCOPE(name)							CL_PROFILE_SCOPE_HIGH(name)

	#define CL_PROFILE_FUNCTION_HIGH()						CL_PROFILE_SCOPE_HIGH(__FUNCSIG__)

	#define CL_PROFILE_FUNCTION()							CL_PROFILE_FUNCTION_HIGH()
#else
	#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filepath)
	#define CL_PROFILE_BEGIN_SESSION(name, filepath)
	#define CL_PROFILE_END_SESSION()
	#define CL_PROFILE_SCOPE_IMPL(name, level)
	#define CL_PROFILE_SCOPE_HIGH(name)
	#define CL_PROFILE_SCOPE(name)
	#define CL_PROFILE_FUNCTION_HIGH()
	#define CL_PROFILE_FUNCTION()
#endif

#if CL_PROFILE > 1
#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filepath)	::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::Medium, filepath);

#define CL_PROFILE_SCOPE_MEDIUM(name)					CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::Medium)

#define CL_PROFILE_FUNCTION_MEDIUM()					CL_PROFILE_SCOPE_MEDIUM(__FUNCSIG__)
#else
	#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filepath)
	#define CL_PROFILE_SCOPE_MEDIUM(name)
	#define CL_PROFILE_FUNCTION_MEDIUM()
#endif

#if CL_PROFILE > 2
	#define CL_PROFILE_BEGIN_SESSION_LOW(name, filepath)	::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::Low, filepath);

	#define CL_PROFILE_SCOPE_LOW(name)						CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::Low)

	#define CL_PROFILE_FUNCTION_LOW()						CL_PROFILE_SCOPE_LOW(__FUNCSIG__)
#else
	#define CL_PROFILE_BEGIN_SESSION_LOW(name, filepath)
	#define CL_PROFILE_SCOPE_LOW(name)
	#define CL_PROFILE_FUNCTION_LOW()
#endif
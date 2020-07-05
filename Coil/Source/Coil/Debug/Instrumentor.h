#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>


namespace Coil
{
	struct ProfileResult
	{
		uint32 Start, End;
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
		RString<> Filepath;
		SessionProfile Profile;
		int64 Created = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	};


	class Instrumentor
	{
	public:
		Instrumentor()
			: CurrentSession(nullptr)
		{}

		void BeginSession(const RString<>& name, SessionProfile profile, const RString<>& filepath = "Profiling.json")
		{
			Profiles.reserve(0x10000);
			CurrentSession = new InstrumentationSession{ name, filepath, profile };
		}

		void EndSession()
		{
			DumpProfiles();
			delete CurrentSession;
			CurrentSession = nullptr;
		}

		int32 RegisterName(const char* name)
		{
			TimerNames.emplace_back(name);
			TimerNames.back()->Replace('"', '\'');

			return TimerNames.size() - 1;
		}

		void CreateProfile(int64 start, int64 end, uint32 threadID, uint32 nameID)
		{
			start -= CurrentSession->Created;
			end -= CurrentSession->Created;
			
			Profiles.push_back({ static_cast<uint32>(start), static_cast<uint32>(end), threadID, nameID });
		}

		[[nodiscard]] bool InSession(SessionProfile profile = SessionProfile::High) const
		{
			return CurrentSession != nullptr && CurrentSession->Profile <= profile;
		}

		void DumpProfiles()
		{
			// TODO Implement multi-threaded parser for raw profiles file

			/*
			OutputStream.open("BinaryDump.bin", std::ios::out | std::ios::binary);
			int64 numberOfNames = TimerNames.size();
			OutputStream.write((char8*)&numberOfNames, sizeof int64);
			for (const auto& name : TimerNames)
			{
				int32 nameLenght = name->GetLength();
				OutputStream.write((char8*)&nameLenght, sizeof int32);
				OutputStream.write(name->CString(), name->GetLength());
			}
			int32 dataOffset = OutputStream.tellp();
			
			OutputStream.write(reinterpret_cast<char8*>(&Profiles[0]), Profiles.size() * sizeof(ProfileResult));
			
			OutputStream.seekp(4, std::ios_base::beg);
			OutputStream.write((char8*)&dataOffset, sizeof int32);
			OutputStream.close();
			*/


			OutputStream.open(CurrentSession->Filepath->CString());

			OutputStream << R"({"otherData": {},"traceEvents":[)";

			for (const auto& profile : Profiles)
			{
				OutputStream << R"({"cat":"function","dur":)"
					<< (profile.End - profile.Start)
					<< R"(,"name":")"
					<< TimerNames.at(profile.NameID)->CString()
					<< R"(","ph":"X","pid":0,"tid":)"
					<< profile.ThreadID
					<< R"(,"ts":)"
					<< profile.Start
					<< "},\n";
			}

			OutputStream << "{}]}";

			OutputStream.flush();
			OutputStream.close();

			Profiles.clear();
			Profiles.shrink_to_fit();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* CurrentSession;
		std::ofstream OutputStream;
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

			StartTimepoint = Stopped ? std::chrono::time_point<std::chrono::high_resolution_clock>{} : std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!Stopped && Instrumentor::Get().InSession())
				Stop();
		}

		void Stop()
		{
			const auto endTimepoint = std::chrono::high_resolution_clock::now();

			const int64 start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch().count();
			const int64 end   = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

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

	#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filepath) ::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::High, filepath);
	#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filepath) ::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::Medium, filepath);
	#define CL_PROFILE_BEGIN_SESSION_LOW(name, filepath) ::Coil::Instrumentor::Get().BeginSession(name, Coil::SessionProfile::Low, filepath);

	#define CL_PROFILE_BEGIN_SESSION(name, filepath) CL_PROFILE_BEGIN_SESSION_HIGH(name, filepath);

	#define CL_PROFILE_END_SESSION() ::Coil::Instrumentor::Get().EndSession();

	#define CL_PROFILE_SCOPE_IMPL(name, level) static int32 TOKENPASTE(nameID, __LINE__) = Coil::Instrumentor::Get().RegisterName(name);\
											Coil::InstrumentationTimer TOKENPASTE(timer, __LINE__)(TOKENPASTE(nameID, __LINE__), level);

	#define CL_PROFILE_SCOPE_HIGH(name) CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::High)
	#define CL_PROFILE_SCOPE_MEDIUM(name) CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::Medium)
	#define CL_PROFILE_SCOPE_LOW(name) CL_PROFILE_SCOPE_IMPL(name, Coil::SessionProfile::Low)

	#define CL_PROFILE_SCOPE(name) CL_PROFILE_SCOPE_HIGH(name)

	#define CL_PROFILE_FUNCTION_HIGH() CL_PROFILE_SCOPE_HIGH(__FUNCSIG__)
	#define CL_PROFILE_FUNCTION_MEDIUM() CL_PROFILE_SCOPE_MEDIUM(__FUNCSIG__)
	#define CL_PROFILE_FUNCTION_LOW() CL_PROFILE_SCOPE_LOW(__FUNCSIG__)

	#define CL_PROFILE_FUNCTION() CL_PROFILE_FUNCTION_HIGH()
#else
	#define CL_PROFILE_BEGIN_SESSION_HIGH(name, filepath)
	#define CL_PROFILE_BEGIN_SESSION_MEDIUM(name, filepath)
	#define CL_PROFILE_BEGIN_SESSION_LOW(name, filepath)
	#define CL_PROFILE_BEGIN_SESSION(name, filepath)
	#define CL_PROFILE_END_SESSION()
	#define CL_PROFILE_SCOPE_IMPL(name, level)
	#define CL_PROFILE_SCOPE_HIGH(name)
	#define CL_PROFILE_SCOPE_MEDIUM(name)
	#define CL_PROFILE_SCOPE_LOW(name)
	#define CL_PROFILE_SCOPE(name)
	#define CL_PROFILE_FUNCTION_HIGH()
	#define CL_PROFILE_FUNCTION_MEDIUM()
	#define CL_PROFILE_FUNCTION_LOW()
	#define CL_PROFILE_FUNCTION()
#endif

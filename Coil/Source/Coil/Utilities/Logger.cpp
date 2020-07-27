#include "pch.h"
#include "Logger.h"


namespace Coil
{
	PointerContainer<Log> Logger::Buffer;

	Log::Log(RString<> message, LogLevel level)
		: Message(Move(message)),
		  Date(Time::Now()),
		  Level(level),
		  Header(LogParser::ComposeHeader(*this)) {}

	RString<> LogParser::Level(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::fatal: return RString("FATAL");
		case LogLevel::error: return RString("ERROR");
		case LogLevel::warning: return RString("WARNING");
		case LogLevel::info: return RString("INFO");
		case LogLevel::debug: return RString("DEBUG");
		case LogLevel::trace: return RString("TRACE");
		default: CL_ASSERT(false, "Unknow log level!");
		}

		return RString("");
	}

	RString<> LogParser::ComposeHeader(const Log& log)
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		auto timeString  = Time::TimestampToString(log.GetDate());
		auto levelString = Level(log.GetLevel());
		return PString("[%R][%R]", &timeString, &levelString);
	}

	Log* Logger::Create(const RString<>& message, LogLevel level)
	{
		CL_PROFILE_FUNCTION_MEDIUM()

		Log* log = new Log(message, level);
		Buffer.Push(log);
		return log;
	}
}

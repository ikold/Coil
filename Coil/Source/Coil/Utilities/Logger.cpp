#include "pch.h"
#include "Logger.h"


namespace Coil
{
	PointerContainer<Log> Logger::Buffer;

	Log::Log(RString<> message, LogLevel level)
		: Message(Move(message)),
		  Date(Time::Now()),
		  Level(level),
		  Header(PString("[%20s][%8s]",
			  Time::TimestampToString(GetDate())->CString(),
			  LogParser::Level(GetLevel())->CString()))
	{}

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

	RString<> LogParser::Compose(const Log& log)
	{
		return PString("[%20s][%8s]",
			Time::TimestampToString(log.GetDate())->CString(),
			Level(log.GetLevel())->CString()
		);
	}

	Log* Logger::Create(const RString<>& message, LogLevel level)
	{
		Log* log = new Log(message, level);
		Buffer.Push(log);
		return log;
	}
}

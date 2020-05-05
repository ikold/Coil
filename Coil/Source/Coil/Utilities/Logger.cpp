#include "pch.h"
#include "Logger.h"

namespace Coil
{
	PointerContainer<Log> Logger::Buffer;

	Log::Log(RString<String> message, LogLevel level)
		: Message(message),
		Date(Time::Now()),
		Level(level),
		Header(PString("[%20s][%8s]",
					   Time::TimestampToString(GetDate())->CString(),
					   LogParser::Level(GetLevel())->CString()
		))
	{}

	RString<String> LogParser::Level(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::fatal:
			return RString("FATAL");
		case LogLevel::error:
			return RString("ERROR");
		case LogLevel::warning:
			return RString("WARNING");
		case LogLevel::info:
			return RString("INFO");
		case LogLevel::debug:
			return RString("DEBUG");
		case LogLevel::trace:
			return RString("TRACE");
		default:
			return RString("");
		}
	}

	RString<String> LogParser::Compose(const Log& log)
	{
		return PString("[%20s][%8s]",
					   Time::TimestampToString(log.GetDate())->CString(),
					   Level(log.GetLevel())->CString(),
					   log.GetMessage()->CString()
		);
	}

	Log* Logger::Create(RString<String> message, LogLevel level)
	{
		Log* log = new Log(message, level);
		Buffer.Push(log);
		return log;
	}
}
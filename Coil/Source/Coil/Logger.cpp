#include "pch.h"
#include "Logger.h"


namespace Coil
{
	PointerContainer<Log> Logger::Buffer;


	Log::Log(const char* message, LogLevel level)
		:Message(message), Level(level), Date(Time::Now())
	{
		FormatedMessage = LogParser::Compose(*this);
	}
	
	std::string LogParser::Level(const LogLevel& level)
	{
		switch (level)
		{
		case Coil::fatal:
			return std::string("FATAL");
		case Coil::error:
			return std::string("ERROR");
		case Coil::warning:
			return std::string("WARNING");
		case Coil::info:
			return std::string("INFO");
		case Coil::debug:
			return std::string("DEBUG");
		case Coil::trace:
			return std::string("TRACE");
		default:
			return "";
		}
	}

	std::string LogParser::Compose(const Log& log)
	{
		std::string s("[");
		s.append(Time::TimestampToString(log.GetDate()));
		s.append("][");
		s.append(Level(log.GetLevel()));
		s.append("] ");
		s.append(log.GetMessage());
		return s;
	}

	Log* Logger::Create(const char* message, LogLevel level)
	{
		Log* log = new Log(message, level);
		Buffer.Push(log);
		return log;
	}
}
#include "pch.h"
#include "Logger.h"


namespace Coil
{
	PointerContainer<Log> Logger::Buffer;


	Log::Log(const RString& message, LogLevel level)
		:Message(message), Level(level), Date(Time::Now())
	{
		FormatedMessage = LogParser::Compose(*this);
	}
	
	RString LogParser::Level(const LogLevel& level)
	{
		switch (level)
		{
		case Coil::fatal:
			return RString("FATAL");
		case Coil::error:
			return RString("ERROR");
		case Coil::warning:
			return RString("WARNING");
		case Coil::info:
			return RString("INFO");
		case Coil::debug:
			return RString("DEBUG");
		case Coil::trace:
			return RString("TRACE");
		default:
			return RString("");
		}
	}

	RString LogParser::Compose(const Log& log)
	{
		std::string s("[");
		s.append(Time::TimestampToString(log.GetDate()));
		s.append("][");
		s.append(Level(log.GetLevel())->CString());
		s.append("] ");
		s.append(log.GetMessage()->CString());
		return RString(s.c_str());
	}

	Log* Logger::Create(const RString& message, LogLevel level)
	{
		Log* log = new Log(message, level);
		Buffer.Push(log);
		return log;
	}
}
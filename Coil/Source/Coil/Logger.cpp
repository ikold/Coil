#include "pch.h"
#include "Logger.h"


namespace Coil
{
	LogContainer Logger::Container;
	
	
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


	LogContainer::~LogContainer()
	{
		for (auto log : Container)
			delete log;
	}

	Log* LogContainer::Create(const char* message, LogLevel level)
	{
		Container.push_back(new Log(message, level));
		
		// temp function for drawing log to console
		printf("%s\n", LogParser::Compose(*Container.back()).c_str());
		
		return Container.back();
	}
}
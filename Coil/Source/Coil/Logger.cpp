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
			break;
		case Coil::error:
			return std::string("ERROR");
			break;
		case Coil::warning:
			return std::string("WARNING");
			break;
		case Coil::info:
			return std::string("INFO");
			break;
		case Coil::debug:
			return std::string("DEBUG");
			break;
		case Coil::trace:
			return std::string("TRACE");
			break;
		default:
			return "";
			break;
		}
	}

	std::string LogParser::Date(const Log& log)
	{
		//return Time::ToString(timestamp, "DD-MM-YYYY");
		return std::string("01-01-1970");
	}

	std::string LogParser::Compose(const Log& log)
	{
		std::string s("[");
		s.append(Time::getTimeStr(log.m_Date));
		s.append("][");
		s.append(Level(log.m_Level));
		s.append("] ");
		s.append(log.m_Message);
		return s;
	}


	LogContainer::~LogContainer()
	{
		for (auto log : Container)
			delete log;
	}

	Log* LogContainer::Create(const char* message, LogLevel level)
	{
		Log* log = new Log(message, level);
		Container.push_back(log);

		// temp function for drawing log to console
		printf("%s\n", LogParser::Compose(*log).c_str());

		return log;
	}

}
#pragma once

#include "Coil/Core.h"

#include "Coil/Time.h"


namespace Coil
{
	/*!	Log level flags used for creating and filltering of logs
	*/
	enum LogLevel
	{
		fatal	= 1 << 0,
		error	= 1 << 1,
		warning	= 1 << 2,
		info	= 1 << 3,
		debug	= 1 << 4,
		trace	= 1 << 5,
	};


	/*!	Struct for Logging system
	*/
	struct COIL_API Log
	{
	public:
		Log(const char* message, LogLevel level)
			:Message(message), Level(level), Date(Time::Now()) {}
		
		inline const char* GetMessage()		const { return Message;	}
		inline const Timestamp GetDate()	const { return Date;	}
		inline const LogLevel GetLevel()	const { return Level;	}
		
	private:
		const char* Message;
		const Timestamp Date;	/*!	Date of creation of log */
		const LogLevel Level;	/*!	Type of log */
	};


	/*!	Class responsible for log formating

		@todo pattern system
	*/
	class COIL_API LogParser
	{
	public:
		/*!	Parser for LogLevel
		
			@param[in]	level		Coil::LogLevel object to be parsed
		
			@return		std::string	LogLevel formated in uppercase string (e.g. "ERROR"), in case of invalid data empty string is returned
		*/
		static std::string Level(const LogLevel& level);
		
		/*!	Creats console ready string from passed Log
		
			@param[in]	log
		
			@return		std::string	currently in format "[YYYY-MM-DD hh-mm-ss][LogLevel]: Message" (e.g. "[2019-12-25 19:23:03][MESSAGE]: Example message")
		*/
		static std::string Compose(const Log& log);
	};


	/*!	Class for storing logs
	*/
	class COIL_API LogContainer
	{
	public:
		LogContainer() {};
		~LogContainer();
		
		/*!	Creates new Log and push it on Container
		
			@param[in]	message
			@param[in]	level
		
			@return		Log*	pointer to created log
		*/
		Log* Create(const char* message, LogLevel level);
	private:
		std::vector<Log*> Container;
	};
	
	
	/*!	Interface for creating and retrieving Logs
	*/
	class COIL_API Logger
	{
	private:
		Logger();
	public:
		
		/*!	@defgroup	Logging methods
			@brief		Methods pass parameters to Container.Create(message, level) to creat and store new Log
		
			@param[in]	message
		
			@return		Log*	pointer to created Log


			@todo parameters passing
		*/
		///@{
		static inline Log* Fatal(const char* message)		{ return Container.Create(message, LogLevel::fatal); }
		static inline Log* Error(const char* message)		{ return Container.Create(message, LogLevel::error); }
		static inline Log* Warning(const char* message)		{ return Container.Create(message, LogLevel::warning); }
		static inline Log* Info(const char* message)		{ return Container.Create(message, LogLevel::info); }
		static inline Log* Debug(const char* message)		{ return Container.Create(message, LogLevel::debug); }
		static inline Log* Trace(const char* message)		{ return Container.Create(message, LogLevel::trace); }
		///@}
		
	private:
		static LogContainer Container;
	};
}


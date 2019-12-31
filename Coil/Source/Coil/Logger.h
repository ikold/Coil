#pragma once

#include "Coil/Core.h"
#include "Coil/Time.h"


namespace Coil
{
	/*!	
	*/
	enum LogLevel
	{
		off,
		fatal,
		error,
		warning,
		info,
		debug,
		trace,
		all
	};


	/*!	Struct for Logging system
	*/
	struct COIL_API Log
	{
	public:
		Log(const char* message, LogLevel level)
			:m_Message(message), m_Level(level), m_Date(Time::Now()) {}

		inline const char* GetMessage() const { return m_Message; }
		inline const Timestamp GetDate() const { return m_Date; }
		inline const LogLevel GetLevel() const { return m_Level; }

	protected:
		const char* m_Message;
		const Timestamp m_Date;
		const LogLevel m_Level;

		friend class LogParser;
	};


	/*! Class responsible for log formating

		@todo pattern system
	*/
	class COIL_API LogParser
	{
	public:
		/*! Parser for LogLevel

			@param[in]	level		Coil::LogLevel object to be parsed

			@return		std::string	LogLevel formated in uppercase string (e.g. "ERROR"), in case of invalid data empty string is returned
		*/
		static std::string Level(const LogLevel& level);
		
		/*! Parser for Log timestamp

			@param[in]	log			Coil::Log object which timestamp is to be parsed

			@return		std::string	Currently returns placeholder string "01-01-1970"

			@todo	replace placeholder with proper parser
		*/
		static std::string Date(const Log& log);

		/*!	Creats console ready string from passed Log

			@param[in]	log

			@return		std::string
		*/
		static std::string Compose(const Log& log);
	};


	class COIL_API LogContainer
	{
	public:
		LogContainer() {};
		~LogContainer();

		/*! Creates new Log and push it on Container

			@param[in]	message
			@param[in]	level

			@return		Log*	pointer to created log
		*/
		Log* Create(const char* message, LogLevel level);
	private:
		std::vector<Log*> Container;
	};


	/*! Interface for creating and retrieving Logs
	*/
	class COIL_API Logger
	{
	public:
		Logger();
		~Logger();

		/*!	@defgroup	Logging methods
			@brief		Methods pass parameters to Conrainer to creat new Log

			@param[in]	message

			@return		Log*	pointer to created Log
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


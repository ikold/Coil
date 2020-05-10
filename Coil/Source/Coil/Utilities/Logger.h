#pragma once

#include <utility>


#include "Coil/Utilities/PointerCointainer.h"


namespace Coil
{
	/*!	Log level flags used for creating and filtering of logs
	*/
	enum class LogLevel
	{
		fatal = 1 << 0,
		error = 1 << 1,
		warning = 1 << 2,
		info = 1 << 3,
		debug = 1 << 4,
		trace = 1 << 5,
	};


	/*!	Struct for Logging system
	*/
	struct Log
	{
	public:
		Log(RString<String> message, LogLevel level);

		[[nodiscard]] RString<String> GetMessage() const { return Message; }
		[[nodiscard]] Timestamp GetDate() const { return Date; }
		[[nodiscard]] LogLevel GetLevel() const { return Level; }

		[[nodiscard]] RString<String> GetHeader() const { return Header; }

	private:
		const RString<String> Message;
		const Timestamp Date;	/*!	Date of creation of log */
		const LogLevel Level;	/*!	Type of log */

		const RString<String> Header;
	};


	/*!	Class responsible for log formatting

		@todo pattern system
	*/
	class LogParser
	{
	public:
		/*!	Parser for LogLevel

			@param[in]	level		Coil::LogLevel object to be parsed

			@return		RString	LogLevel formatted in uppercase string (e.g. "ERROR"), in case of invalid Data empty string is returned
		*/
		static RString<String> Level(LogLevel level);

		/*!	Creates console ready string from passed Log

			@param[in]	log

			@return		RString	currently in format "[YYYY-MM-DD hh-mm-ss][LogLevel]: Message" (e.g. "[2019-12-25 19:23:03][MESSAGE]: Example message")
		*/
		static RString<String> Compose(const Log& log);
	};


	/*!	Interface for creating and retrieving Logs
	*/
	class Logger
	{
	public:
		Logger() = delete;

		/*!	@defgroup	Logging methods
			@brief		Methods pass parameters to Container.Create(message, level) to create and store new Log

			@param[in]	message

			@return		Log*	pointer to created Log

			@todo parameters passing
		*/
		///@{
		static Log* Fatal(RString<String> message) { return Create(std::move(message), LogLevel::fatal); }
		static Log* Error(RString<String> message) { return Create(std::move(message), LogLevel::error); }
		static Log* Warning(RString<String> message) { return Create(std::move(message), LogLevel::warning); }
		static Log* Info(RString<String> message) { return Create(std::move(message), LogLevel::info); }
		static Log* Debug(RString<String> message) { return Create(std::move(message), LogLevel::debug); }
		static Log* Trace(RString<String> message) { return Create(std::move(message), LogLevel::trace); }
		///@}

		static PointerContainer<Log>* GetBuffer() { return &Buffer; }

	private:
		/*!	Creates new Log and push it on Container

			@param[in]	message
			@param[in]	level

			@return		Log*	pointer to created log
		*/
		static Log* Create(RString<String> message, LogLevel level);

	private:
		static PointerContainer<Log> Buffer;
	};
}

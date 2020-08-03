#pragma once

#include "Coil/Utilities/PointerCointainer.h"


namespace Coil
{
	/**
	 * @brief Log level flags used for creating and filtering of logs
	 */
	enum class LogLevel
	{
		fatal	= 1 << 0,
		error	= 1 << 1,
		warning = 1 << 2,
		info	= 1 << 3,
		debug	= 1 << 4,
		trace	= 1 << 5,
	};


	/**
	 * @brief Struct for Logging system
	 */
	struct Log
	{
	public:
		Log(RString<> message, LogLevel level);

		#undef GetMessage // defined in WinUser.h
		[[nodiscard]] RString<> GetMessage() const { return Message; }
		[[nodiscard]] Timestamp GetDate() const { return Date; }
		[[nodiscard]] LogLevel GetLevel() const { return Level; }

		[[nodiscard]] RString<> GetHeader() const { return Header; }

	private:
		const RString<> Message;
		/** Creation timestamp */
		const Timestamp Date;
		const LogLevel Level;

		/** Cached header */
		const RString<> Header;
	};


	/**
	 * @brief Class responsible for log formatting
	 * 
	 * @todo pattern system
	 */
	class LogParser
	{
	public:
		/**
		 * @brief Parser for LogLevel
		 * 
		 * @param[in]	level		Coil::LogLevel object to be parsed
		 * 
		 * @return		RString	LogLevel formatted in uppercase string (e.g. "ERROR"), in case of invalid Data empty string is returned
		 */
		static RString<> Level(LogLevel level);

		/**
		 * @brief Creates console ready string from passed Log
		 * 
		 * @param[in]	log
		 * 
		 * @return		RString	currently in format "[YYYY-MM-DD hh-mm-ss][LogLevel]: Message" (e.g. "[2019-12-25 19:23:03][MESSAGE]: Example message")
		 */
		static RString<> ComposeHeader(const Log& log);
	};


	/**
	 * @brief Interface for creating and retrieving Logs
	 *
	 * @bug Might throw exception if buffer is modified at the same time as reading or iterating on another thread
	 */
	class Logger
	{
	public:
		Logger() = delete;

		/**
		 * @name Logging methods
		 * 
		 * @brief Wrappers for private method Create(const RString<>&, LogLevel)
		 * 
		 * @param[in]	message
		 * 
		 * @return		Pointer to created Log
		 */
		///@{
		static Log* Fatal(const RString<>& message) { return Create(message, LogLevel::fatal); }
		static Log* Error(const RString<>& message) { return Create(message, LogLevel::error); }
		static Log* Warning(const RString<>& message) { return Create(message, LogLevel::warning); }
		static Log* Info(const RString<>& message) { return Create(message, LogLevel::info); }
		static Log* Debug(const RString<>& message) { return Create(message, LogLevel::debug); }
		static Log* Trace(const RString<>& message) { return Create(message, LogLevel::trace); }
		///@}

		/**
		 * @name Logging methods with parameters handling
		 *
		 * @brief Wrappers for private method Create(const RString<>&, LogLevel)
		 *
		 * @param[in]	message
		 * @param[in]	args	parameters for the message
		 *
		 * @return		Pointer to created Log
		 *
		 * @see Coil::PString for details about message format
		 * @note There is no way to change parameters after the log creation, to do so pass RString<PString> to alternative logging method
		 */
		///@{
		template<typename... Args>
		static Log* Fatal(const char8* message, Args ... args) { return Create(PString(message, args...).ToString(), LogLevel::fatal); }

		template<typename... Args>
		static Log* Error(const char8* message, Args ... args) { return Create(PString(message, args...).ToString(), LogLevel::error); }

		template<typename... Args>
		static Log* Warning(const char8* message, Args ... args) { return Create(PString(message, args...).ToString(), LogLevel::warning); }

		template<typename... Args>
		static Log* Info(const char8* message, Args ... args) { return Create(PString(message, args...).ToString(), LogLevel::info); }

		template<typename... Args>
		static Log* Debug(const char8* message, Args ... args) { return Create(PString(message, args...).ToString(), LogLevel::debug); }

		template<typename... Args>
		static Log* Trace(const char8* message, Args ... args) { return Create(PString(message, args...).ToString(), LogLevel::trace); }

		///@}

		static PointerContainer<Log>* GetBuffer() { return &Buffer; }

	private:
		/**
		 * @brief Creates new Log and push it on Container
		 * 
		 * @param[in]	message
		 * @param[in]	level
		 * 
		 * @return		Pointer to created log
		 */
		static Log* Create(const RString<>& message, LogLevel level);

	private:
		static PointerContainer<Log> Buffer;
	};
}

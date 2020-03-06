#pragma once

#include "Coil/Core.h"

#include "Coil/PointerCointainer.h"
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
		Log(RString message, LogLevel level);
		
		inline RString GetMessage()	const { return Message;	}
		inline Timestamp GetDate()	const { return Date;	}
		inline LogLevel GetLevel()	const { return Level;	}
		
		inline RString GetFormatedMessage() const { return FormatedMessage; }

	private:
		const RString Message;
		const Timestamp Date;	/*!	Date of creation of log */
		const LogLevel Level;	/*!	Type of log */

		const RString FormatedMessage;
	};


	/*!	Class responsible for log formating

		@todo pattern system
	*/
	class COIL_API LogParser
	{
	public:
		/*!	Parser for LogLevel
		
			@param[in]	level		Coil::LogLevel object to be parsed
		
			@return		RString	LogLevel formated in uppercase string (e.g. "ERROR"), in case of invalid Data empty string is returned
		*/
		static RString Level(LogLevel level);
		
		/*!	Creats console ready string from passed Log
		
			@param[in]	log
		
			@return		RString	currently in format "[YYYY-MM-DD hh-mm-ss][LogLevel]: Message" (e.g. "[2019-12-25 19:23:03][MESSAGE]: Example message")
		*/
		static RString Compose(const Log& log);
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
		static inline Log* Fatal(RString message)		{ return Create(message, LogLevel::fatal); }
		static inline Log* Error(RString message)		{ return Create(message, LogLevel::error); }
		static inline Log* Warning(RString message)		{ return Create(message, LogLevel::warning); }
		static inline Log* Info(RString message)		{ return Create(message, LogLevel::info); }
		static inline Log* Debug(RString message)		{ return Create(message, LogLevel::debug); }
		static inline Log* Trace(RString message)		{ return Create(message, LogLevel::trace); }
		///@}

		static PointerContainer<Log>* GetBuffer() { return &Buffer; }

	private:
		/*!	Creates new Log and push it on Container

			@param[in]	message
			@param[in]	level

			@return		Log*	pointer to created log
		*/
		static Log* Create(RString message, LogLevel level);
		
	private:
		static PointerContainer<Log> Buffer;
	};
}


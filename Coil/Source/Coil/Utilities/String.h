#pragma once

#include "Coil/Core.h"

namespace Coil
{
	class COIL_API String
	{
	public:
		String();
		String(const char8* text);
		String(const String& string);
		~String();

		inline char8* CString() const { return Data; }

		inline int32 GetLength() const { return Length; }

		String& operator=(const char8* str);
		String& operator=(const String& str);

	protected:
		static int32 CStringLength(const char8* str);

	protected:
		char8* Data;

		int32 Length;
	};

	class COIL_API RString
	{
	public:
		RString();
		RString(const String& string);
		RString(const RString& rString);
		RString(const char8* text);
		~RString();

		String* operator->() const { return Get(); }

		RString& operator=(const char8* str);
		RString& operator=(const String& str);
		RString& operator=(const RString& str);

		inline String& operator*() const { return *Get(); }

		static inline RString Copy(const RString& str);

	private:
		inline String* Get() const { return StringPointer; }

		void DerefencString();

	private:
		String* StringPointer;
		int* Counter;
	};

	class COIL_API SString : public String
	{
	public:
		SString();
		SString(const char8* text);
		~SString();

		void Reserv(int32 size);

		void Shrink();

		SString& operator<<(const char8* str);
		SString& operator<<(const String& str);
		SString& operator<<(const RString& str);
		SString& operator<<(const SString& str);

	private:
		void Append(const char8* str, int32 size);

	private:
		int32 Size;
	};
}
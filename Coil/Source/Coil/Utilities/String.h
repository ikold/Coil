#pragma once

#include "Coil/Core.h"

namespace Coil
{
	class COIL_API String
	{
	public:
		String();
		String(const char* text);
		String(const String& string);
		~String();

		void Test()
		{
			;
		}

		inline char* CString() { return Data; }

	protected:
		static int CStringLength(const char* str);

		inline static void CStringCopy(const char* sStr, char* dStr) { while (*dStr++ = *sStr++); }

	private:
		char* Data;

		int Size;
	};

	class COIL_API RString
	{
	public:
		RString();
		RString(const String& string);
		RString(const RString& rString);
		RString(const char * text);
		~RString();

		String* operator->() const { return Get(); }

		RString& operator=(const RString& rString);

		inline static RString Copy(const RString& rString);

	private:
		inline String* Get() const { return StringPointer; }

		void DerefencString();

	private:
		String* StringPointer;

		int* Counter;
	};
}
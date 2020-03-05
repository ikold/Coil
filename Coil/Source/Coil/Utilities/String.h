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

		inline char* CString() const { return Data; }

		inline int GetLength() const { return Length; }

		String& operator=(const char* str);
		String& operator=(const String& str);

	protected:
		static int CStringLength(const char* str);

	protected:
		char* Data;

		int Length;
	};

	class COIL_API RString
	{
	public:
		RString();
		RString(const String& string);
		RString(const RString& rString);
		RString(const char* text);
		~RString();

		String* operator->() const { return Get(); }

		RString& operator=(const char* str);
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
		SString(const char* text);
		~SString();

		void Reserv(int size);

		void Shrink();

		SString& operator<<(const char* str);
		SString& operator<<(const String& str);
		SString& operator<<(const RString& str);
		SString& operator<<(const SString& str);

	private:
		void Append(const char* str, int size);

	private:
		int Size;
	};
}
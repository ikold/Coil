#include "pch.h"
#include "String.h"

namespace Coil
{
	String::String()
		: Data(nullptr)
	{
	}

	String::String(const char* text)
	{
		Size = CStringLength(text);
		Data = new char[Size + 1];
		memcpy(Data, text, Size + 1);
	}

	String::String(const String& string)
	{
		Size = string.Size;
		Data = new char[Size + 1];
		memcpy(Data, string.Data, Size + 1);
	}

	String::~String()
	{
		delete Data;
	}

	int String::CStringLength(const char* str)
	{
		int length = 0;
		while (*str++)
			++length;

		return length;
	}


	RString::RString()
	{
		StringPointer = new String();
		Counter = new int(1);
	}

	RString::RString(const String& string)
	{
		StringPointer = new String(string);
		Counter = new int(1);
	}

	RString::RString(const RString& rString)
	{
		StringPointer = rString.StringPointer;
		Counter = rString.Counter;
		++(*Counter);
	}

	RString::RString(const char* text)
	{
		StringPointer = new String(text);
		Counter = new int(1);
	}

	RString::~RString()
	{
		DerefencString();
	}

	RString& RString::operator=(const RString& rString)
	{
		DerefencString();

		StringPointer = rString.StringPointer;
		Counter = rString.Counter;
		++(*Counter);

		return *this;
	}

	inline RString RString::Copy(const RString& rString)
	{
		return RString(*rString.Get());
	}

	void RString::DerefencString()
	{
		if (--(*Counter) == 0)
		{
			delete StringPointer;
			delete Counter;
		}

		// Could be deleted if referenc assigment is assured
		Counter = nullptr;
		StringPointer = nullptr;
	}
}
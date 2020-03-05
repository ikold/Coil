#include "pch.h"
#include "String.h"

namespace Coil
{
	String::String()
	{
		Data = new char[1];
		*Data = '\0';
		Length = 0;
	}

	String::String(const char* text)
	{
		Length = CStringLength(text);
		Data = new char[Length + 1];
		memcpy(Data, text, Length + 1);
	}

	String::String(const String& string)
	{
		Length = string.Length;
		Data = new char[Length + 1];
		memcpy(Data, string.Data, Length + 1);
	}

	String::~String()
	{
		delete[] Data;
	}

	String& String::operator=(const char* str)
	{
		delete[] Data;

		Length = CStringLength(str);
		Data = new char[Length + 1];
		memcpy(Data, str, Length + 1);

		return *this;
	}

	String& String::operator=(const String& str)
	{
		delete[] Data;

		Length = str.Length;
		Data = new char[Length + 1];
		memcpy(Data, str.Data, Length + 1);

		return *this;
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
		StringPointer = new String;
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

	RString& RString::operator=(const char* str)
	{
		DerefencString();
		StringPointer = new String(str);
		Counter = new int(1);
		return *this;
	}

	RString& RString::operator=(const String& str)
	{
		DerefencString();
		StringPointer = new String(str);
		Counter = new int(1);
		return *this;
	}

	RString& RString::operator=(const RString& str)
	{
		// Failsafe for self assigning
		String* stringPointer = str.StringPointer;
		int* counter = str.Counter;
		++(*counter);

		DerefencString();
		StringPointer = stringPointer;
		Counter = counter;
		return *this;
	}

	inline RString RString::Copy(const RString& str)
	{
		return RString(*str);
	}

	void RString::DerefencString()
	{
		if (--(*Counter) == 0)
		{
			delete StringPointer;
			delete Counter;
		}

		// Could be removed if reference assigment is assured
		Counter = nullptr;
		StringPointer = nullptr;
	}


	SString::SString()
		: Size(Length)
	{
	}

	SString::SString(const char* text)
		: String(text), Size(Length)
	{
	}

	SString::~SString()
	{
	}

	void SString::Reserv(int size)
	{
		Size = size;
		char* newData = new char[size + 1];
		memcpy(newData, Data, Length + 1);
		delete[] Data;
		Data = newData;
	}

	void SString::Shrink()
	{
		if (Length == Size)
			return;

		char* newData = new char[Length + 1];
		memcpy(newData, Data, Length + 1);
		delete[] Data;
		Data = newData;

	}

	SString& SString::operator<<(const char* str)
	{
		Append(str, CStringLength(str));
		return *this;
	}

	SString& SString::operator<<(const String& str)
	{
		Append(str.CString(), str.GetLength());
		return *this;
	}

	SString& SString::operator<<(const RString& str)
	{
		Append(str->CString(), str->GetLength());
		return *this;
	}
	
	SString& SString::operator<<(const SString& str)
	{
		Append(str.CString(), str.GetLength());
		return *this;
	}

	void SString::Append(const char* str, int size)
	{
		if (Length + size <= Size)
			memcpy(Data + Length, str, size + 1);
		else
		{
			Size = Length + size;
			char* newData = new char[Size + 1];

			memcpy(newData, Data, Length);
			memcpy(newData + Length, str, size + 1);

			delete[] Data;
			Data = newData;
		}
		Length += size;
	}
}
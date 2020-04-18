#include "pch.h"
#include "String.h"

#include "ryu/ryu.h"
#include <cstdarg>


namespace Coil
{
	String::String()
	{
		Data = new char[1];
		*Data = '\0';
		Length = 0;
	}

	String::String(const String& string)
	{
		Length = string.Length;
		Data = new char[Length + 1];
		memcpy(Data, string.Data, Length + 1);
	}

	String::String(const char8* text ...)
	{
		va_list args;
		va_start(args, text);

		int32 insert = false;
		std::vector<int32> inserts;
		std::vector<RString> insertsString;

		for (int32 i = 0; text[i] != '\0'; ++i)
		{
			if (!insert)
			{
				switch (text[i])
				{
				case '\\':
					++i;
					break;
				case '%':
					inserts.push_back(i);
					insert = true;
					break;
				}
			}
			else
			{
				switch (text[i])
				{
				case 's':
					insertsString.push_back(va_arg(args, char8*));
					break;
				case 'd':
					insertsString.push_back(String::Convert(va_arg(args, int32)));
					break;
				case 'f':
					insertsString.push_back(String::Convert(va_arg(args, float64), 3));
					break;
				case '%':
				default:
					inserts.pop_back();
					break;
				}

				insert = false;
			}
		}
		
		va_end(args);


		Length = CStringLength(text) - insertsString.size() * 2;

		for (RString itr : insertsString)
			Length += itr->GetLength();

		Data = new char8[Length + 1];

		int32 sourceOffset = 0;
		int32 dataOffset = 0;

		for (int i = 0; i < inserts.size(); ++i)
		{
			memcpy(Data + dataOffset, text + sourceOffset, inserts[i] - sourceOffset);
			dataOffset += inserts[i] - sourceOffset;
			sourceOffset += inserts[i] - sourceOffset + 2;
			memcpy(Data + dataOffset, insertsString[i]->CString(), insertsString[i]->GetLength());
			dataOffset += insertsString[i]->GetLength();
		}

		memcpy(Data + dataOffset, text + sourceOffset, Length - dataOffset + 1);
	}

	String::~String()
	{
		delete[] Data;
	}

	void String::Reverse()
	{
		char* forwardIterator = Data;
		char* backwardIterator = Data + Length - 1;
		char tmp;
		while (forwardIterator < backwardIterator)
		{
			tmp = *forwardIterator;
			*forwardIterator = *backwardIterator;
			*backwardIterator = tmp;
			++forwardIterator;
			--backwardIterator;
		}
	}

	String& String::operator=(const char8* str)
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

	inline String String::Convert(float32 value, int fractionLength)
	{
		return Convert((float64)value, fractionLength);
	}

	inline String String::Convert(float64 value, int fractionLength)
	{
		return d2fixed(value, fractionLength);
	}

	String String::Convert(void* address)
	{
		String addressString = Convert<uint64>((uint64)address, 16);

		SString formatedAddress;
		formatedAddress.Reserve(18);
		formatedAddress << "0x";

		for (int i = 16 - addressString.GetLength(); i > 0; --i)
			formatedAddress << "0";

		return formatedAddress << addressString;
	}

	int32 String::CStringLength(const char8* str)
	{
		int32 length = 0;
		while (*str++)
			++length;

		return length;
	}


	RString::RString()
	{
		StringPointer = new String;
		Counter = new int32(1);
	}

	RString::RString(const String& string)
	{
		StringPointer = new String(string);
		Counter = new int32(1);
	}

	RString::RString(const RString& rString)
	{
		StringPointer = rString.StringPointer;
		Counter = rString.Counter;
		++(*Counter);
	}

	RString::RString(const char8* text)
	{
		StringPointer = new String(text);
		Counter = new int32(1);
	}

	RString::~RString()
	{
		DerefencString();
	}

	RString& RString::operator=(const char8* str)
	{
		DerefencString();
		StringPointer = new String(str);
		Counter = new int32(1);
		return *this;
	}

	RString& RString::operator=(const String& str)
	{
		DerefencString();
		StringPointer = new String(str);
		Counter = new int32(1);
		return *this;
	}

	RString& RString::operator=(const RString& str)
	{
		// Failsafe for self assigning
		String* stringPointer = str.StringPointer;
		int32* counter = str.Counter;
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

	SString::SString(const char8* text)
		: String(text), Size(Length)
	{
	}

	SString::~SString()
	{
	}

	void SString::Reserve(int32 size)
	{
		Size = size;
		char8* tmp = (char8*)realloc(Data, Size + 1);
		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
			Data = tmp;
	}

	void SString::Shrink()
	{
		if (Length == Size)
			return;

		char8* tmp = (char8*)realloc(Data, Length + 1);
		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
			Data = tmp;

		Size = Length;
	}

	SString& SString::operator<<(const char8* str)
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

	void SString::Append(const char8* str, int32 size)
	{
		if (Length + size <= Size)
			memcpy(Data + Length, str, size + 1);
		else
		{
			Size = Length + size;

			char8* tmp = (char8*)realloc(Data, Size + 1);
			CL_ASSERT(tmp, "Failed to reallocate memory");
			if (tmp)
				Data = tmp;

			memcpy(Data + Length, str, size + 1);
		}
		Length += size;
	}
}
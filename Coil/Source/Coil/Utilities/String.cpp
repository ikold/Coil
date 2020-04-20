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

	String::String(String&& string) noexcept
		: Data(std::exchange(string.Data, nullptr)),
		Length(string.Length)
	{
	}

	String::String(const char8* text)
	{
		Length = CStringLength(text);
		Data = new char[Length + 1];
		memcpy(Data, text, Length + 1);
	}

	String::String(const PString& string)
	{
		Length = string.GetLength();
		Data = new char[Length + 1];

		char8* iteratorSrc = string.CString();
		char8* iteratorDst = Data;
		do
		{
			if (*iteratorSrc != 127)
			{
				*iteratorDst = *iteratorSrc;
				++iteratorDst;
			}
		} while (*iteratorSrc++);
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
		return *this = String(str);
	}

	String& String::operator=(const String& str)
	{
		return *this = String(str);
	}

	String& String::operator=(String&& str) noexcept
	{
		swap(*this, str);
		return *this;
	}

	inline String String::Convert(float32 value, int fractionLength)
	{
		return Convert((float64)value, fractionLength);
	}

	String String::Convert(float64 value, int fractionLength)
	{
		char* fString = d2fixed(value, fractionLength);
		String rString(fString);
		delete[] fString;
		return rString;
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

	RString::RString(RString&& rString)
		: StringPointer(std::move(rString.StringPointer)),
		Counter(std::move(rString.Counter))
	{
		++(*Counter);
	}

	RString::~RString()
	{
		DerefencString();
	}

	RString& RString::operator=(const char8* str)
	{
		swap(*this, RString(str));
		return *this;
	}

	RString& RString::operator=(const RString& str)
	{
		return *this = RString(str);
	}

	RString& RString::operator=(RString&& str)
	{
		swap(*this, str);
		return *this;
	}

	inline RString RString::Copy()
	{
		return RString(*StringPointer);
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

	PString::PString(char8* text ...)
	{
		va_list args;
		va_start(args, text);

		int32 insert = false;

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
					InsertIndex.push_back(i);
					insert = true;
					break;
				}
			}
			else
			{
				switch (text[i])
				{
				case 's':
				case 'd':
				case 'f':
					InsertType.push_back(text[i]);
					break;
				case '%':
				default:
					InsertIndex.pop_back();
					break;
				}

				insert = false;
			}
		}


		Size = CStringLength(text) + 1 - InsertIndex.size() * 2;

		for (char8 type : InsertType)
			Size += TypeToSize(type);

		Data = new char8[Size];

		int32 sourceOffset = 0;
		int32 dataOffset = 0;
		int32 paddingOffset = 0;

		for (int i = 0; i < InsertIndex.size(); ++i)
		{
			int32 padingSize = TypeToSize(InsertType[i]);
			memcpy(Data + dataOffset, text + sourceOffset, InsertIndex[i] - sourceOffset);
			dataOffset += InsertIndex[i] - sourceOffset;
			sourceOffset += InsertIndex[i] - sourceOffset + 2;
			memset(Data + dataOffset, 127, padingSize);
			dataOffset += padingSize;
			InsertIndex[i] += paddingOffset - 2 * i;
			paddingOffset += padingSize;
		}

		memcpy(Data + dataOffset, text + sourceOffset, Size - dataOffset);

		int32 elementIndex = 0;
		for (char8 type : InsertType)
		{
			switch (type)
			{
			case 's':
				Set(elementIndex, va_arg(args, char8*));
				break;
			case 'd':
				Set(elementIndex, va_arg(args, int32));
				break;
			case 'f':
				Set(elementIndex, va_arg(args, float64));
				break;
			}

			++elementIndex;
		}

		va_end(args);
	}

	int32 PString::TypeToSize(char8 type) const
	{
		switch (type)
		{
		case 's':
			return 32;
		case 'd':
			return 12;
		case 'f':
			return 32;
		}
	}

	void PString::Set(int32 elementIndex, char8* text)
	{
		int32 size = CStringLength(text);

		char8* index = Data + InsertIndex[elementIndex];
		memset(index, 127, TypeToSize('s'));
		memcpy(index, text, size);

		CalculateLength();
	}

	void PString::Set(int32 elementIndex, int32 value)
	{
		char8* iterator = Data + InsertIndex[elementIndex];
		memset(iterator, 127, TypeToSize('d'));

		iterator += TypeToSize('d') - 1;

		// using ~ bitwise operator for getting rid of negative sign
		uint64 operationalValue = (value > 0)?value:(1 + (uint64)~value);

		do
		{
			switch (operationalValue % 10)
			{
			case 0:
				*iterator-- = '0';
				break;
			case 1:
				*iterator-- = '1';
				break;
			case 2:
				*iterator-- = '2';
				break;
			case 3:
				*iterator-- = '3';
				break;
			case 4:
				*iterator-- = '4';
				break;
			case 5:
				*iterator-- = '5';
				break;
			case 6:
				*iterator-- = '6';
				break;
			case 7:
				*iterator-- = '7';
				break;
			case 8:
				*iterator-- = '8';
				break;
			case 9:
				*iterator-- = '9';
				break;
			case 10:
				*iterator-- = 'A';
				break;
			case 11:
				*iterator-- = 'B';
				break;
			case 12:
				*iterator-- = 'C';
				break;
			case 13:
				*iterator-- = 'D';
				break;
			case 14:
				*iterator-- = 'E';
				break;
			case 15:
				*iterator-- = 'F';
				break;
			}

			operationalValue /= 10;
		} while (operationalValue);


		if (value < 0)
		{
			*iterator = '-';
		}

		CalculateLength();
	}

	void PString::Set(int32 elementIndex, float64 value)
	{
		char8* fString = d2fixed(value, 3);
		int32 size = CStringLength(fString);

		char8* index = Data + InsertIndex[elementIndex];
		memset(index, 127, TypeToSize('f'));
		memcpy(index, fString, size);

		delete[] fString;

		CalculateLength();
	}

	void PString::CalculateLength()
	{
		Length = Size - 1;
		char8* iterator = Data;
		do
		{
			if (*iterator == 127)
				--Length;
		} while ((*iterator++));
		int32 a = GetLength();
	}
}
#include "pch.h"
#include "String.h"

#include "ryu/ryu.h"
#include <cstdarg>

namespace Coil
{
	String::String()
	{
		Data = new char[1]();
		Length = 0;
	}

	String::String(const String& string)
	{
		Length = string.Length;
		Data = new char[(int64)Length + 1];
		memcpy(Data, string.Data, (int64)Length + 1);
	}

	String::String(String&& string) noexcept
		: Data(std::exchange(string.Data, nullptr)),
		Length(string.Length)
	{}


	String::String(const char8* text)
	{
		Length = CStringLength(text);
		Data = new char[(int64)Length + 1];
		memcpy(Data, text, (int64)Length + 1);
	}

	String::String(const char8* text, int32 length)
		: Length(length)
	{
		Data = new char[(int64)Length + 1];
		memcpy(Data, text, (int64)Length + 1);
	}


	String::~String()
	{
		delete[] Data;
	}


	String& String::operator=(const String& string)
	{
		return *this = String(string);
	}

	String& String::operator=(String&& string) noexcept
	{
		swap(*this, string);
		return *this;
	}

	String& String::operator=(const char8* string)
	{
		return *this = String(string);
	}


	void swap(String& left, String& right) noexcept
	{
		using std::swap;

		swap(left.Data, right.Data);
		swap(left.Length, right.Length);
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


	String String::Convert(uint64 value, int32 base)
	{
		SString reverseString;
		reverseString.Reserve(20); // 20 - Maximum number of digits in 64 bit int

		// using ~ bitwise operator for getting rid of negative sign
		uint64 operationalValue = (value > 0) ? value : (1 + (uint64)~value);

		do
		{
			switch (operationalValue % base)
			{
			case 0:
				reverseString << "0";
				break;
			case 1:
				reverseString << "1";
				break;
			case 2:
				reverseString << "2";
				break;
			case 3:
				reverseString << "3";
				break;
			case 4:
				reverseString << "4";
				break;
			case 5:
				reverseString << "5";
				break;
			case 6:
				reverseString << "6";
				break;
			case 7:
				reverseString << "7";
				break;
			case 8:
				reverseString << "8";
				break;
			case 9:
				reverseString << "9";
				break;
			case 10:
				reverseString << "A";
				break;
			case 11:
				reverseString << "B";
				break;
			case 12:
				reverseString << "C";
				break;
			case 13:
				reverseString << "D";
				break;
			case 14:
				reverseString << "E";
				break;
			case 15:
				reverseString << "F";
				break;
			}

			operationalValue /= base;
		} while (operationalValue);

		// Adding '-' if needed, at end of string before reversing
		if (value < 0)
		{
			reverseString << "-";
		}

		reverseString.Shrink();

		reverseString.Reverse();
		return reverseString;
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
		String addressString = Convert((uint64)address, 16);

		SString formatedAddress;
		formatedAddress.Reserve(18);
		formatedAddress << "0x";

		for (int i = 16 - addressString.GetLength(); i > 0; --i)
			formatedAddress << "0";

		return formatedAddress << addressString;
	}


	int32 String::CStringLength(const char8* string)
	{
		int32 length = 0;
		while (*string++)
			++length;

		return length;
	}


	SString::SString()
		: Size(Length)
	{}

	SString::SString(const SString& string)
		: String(string.Data, string.Size),
		Size(string.Size)
	{
		Length = string.Length;
	}

	SString::SString(SString&& string) noexcept
		: String((String&&)string),
		Size(string.Size)
	{}

	SString::SString(const char8* text)
		: String(text),
		Size(Length)
	{}

	SString::SString(const char8* text, int32 length)
		: String(text, length),
		Size(Length)
	{}


	SString::~SString()
	{}


	SString& SString::operator=(const SString& string)
	{
		return *this = SString(string);
	}

	SString& SString::operator=(SString&& string) noexcept
	{
		swap(*this, string);
		return *this;
	}


	void swap(SString& left, SString& right) noexcept
	{
		using std::swap;

		swap((String&)left, (String&)right);
		swap(left.Size, right.Size);
	}

	void SString::Reserve(int32 size)
	{
		Size = size;
		char8* tmp = (char8*)realloc(Data, (int64)Size + 1);

		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
			Data = tmp;
	}

	void SString::Shrink()
	{
		if (Length == Size)
			return;

		char8* tmp = (char8*)realloc(Data, (int64)Length + 1);

		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
			Data = tmp;

		Size = Length;
	}


	SString& SString::operator<<(const char8* string)
	{
		Append(string, CStringLength(string));
		return *this;
	}

	SString& SString::operator<<(const String& string)
	{
		Append(string.CString(), string.GetLength());
		return *this;
	}


	void SString::Append(const char8* string, int32 size)
	{
		if (Length + size <= Size)
			memcpy(Data + Length, string, (int64)size + 1);
		else
		{
			Size = Length + size;

			char8* tmp = (char8*)realloc(Data, (int64)Size + 1);
			CL_ASSERT(tmp, "Failed to reallocate memory");
			if (tmp)
				Data = tmp;

			memcpy(Data + Length, string, (int64)size + 1);
		}
		Length += size;
	}


	PString::PString()
		: Size(Length)
	{}

	PString::PString(const PString& string)
		: String(string.Data, string.Size),
		Size(string.Size),
		InsertIndex(string.InsertIndex),
		InsertType(string.InsertType)
	{
		Length = string.Length;
	}

	PString::PString(PString&& string) noexcept
		: String((String&&)string),
		Size(string.Size),
		InsertIndex(std::move(string.InsertIndex)),
		InsertType(std::move(string.InsertType))
	{}

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

		Size = CStringLength(text) + 1 - (int32)InsertIndex.size() * 2;

		for (char8 type : InsertType)
			Size += TypeToSize(type);

		delete[] Data;
		Data = new char8[Size];

		int32 sourceOffset = 0;
		int32 dataOffset = 0;
		int32 paddingOffset = 0;

		for (int i = 0; i < InsertIndex.size(); ++i)
		{
			int32 padingSize = TypeToSize(InsertType[i]);
			memcpy(Data + dataOffset, text + sourceOffset, (int64)InsertIndex[i] - sourceOffset);
			dataOffset += InsertIndex[i] - sourceOffset;
			sourceOffset += InsertIndex[i] - sourceOffset + 2;
			memset(Data + dataOffset, 127, padingSize);
			dataOffset += padingSize;
			InsertIndex[i] += paddingOffset - 2 * i;
			paddingOffset += padingSize;
		}

		memcpy(Data + dataOffset, text + sourceOffset, (int64)Size - dataOffset);

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


	PString::~PString()
	{}


	PString& PString::operator=(const PString& string)
	{
		return *this = PString(string);
	}

	PString& PString::operator=(PString&& string) noexcept
	{
		swap(*this, string);
		return *this;
	}


	void swap(PString& left, PString& right) noexcept
	{
		using std::swap;

		swap((String&)left, (String&)right);
		swap(left.Size, right.Size);
		swap(left.InsertIndex, right.InsertIndex);
		swap(left.InsertType, right.InsertType);
	}


	String PString::ToString() const
	{
		int length = GetLength();
		char8* newData = new char[(int64)Length + 1];

		char8* iteratorSrc = Data;
		char8* iteratorDst = newData;
		do
		{
			if (*iteratorSrc != 127)
			{
				*iteratorDst = *iteratorSrc;
				++iteratorDst;
			}
		} while (*iteratorSrc++);
		return String(newData, length);
	}

	int32 PString::TypeToSize(char8 type) const
	{
		switch (type)
		{
		case 's':
			return 64;
		case 'd':
			return 12;
		case 'f':
			return 32;
		}

		return 0;
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

		iterator += (int64)TypeToSize('d') - 1;

		// using ~ bitwise operator for getting rid of negative sign
		uint64 operationalValue = (value > 0) ? value : (1 + (uint64)~value);

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
		char8* index = Data + InsertIndex[elementIndex];
		memset(index, 127, TypeToSize('f'));

		d2fixed_buffered_n(value, 3, index);

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
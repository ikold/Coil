#include "pch.h"
#include "String.h"

#include "ryu/ryu.h"
#include <cstdarg>


namespace Coil
{
	String::String()
		: Data(nullptr),
		  Length(0),
		  Size(Length)	{}

	String::String(const String& string)
		: Length(string.Length),
		  Size(Length)
	{
		Data = new char[static_cast<size_t>(Size) + 1];
		memcpy(Data, string.Data, static_cast<size_t>(Size) + 1);
	}

	String::String(String&& string) noexcept
		: Data(std::exchange(string.Data, nullptr)),
		  Length(std::exchange(string.Length, 0)),
		  Size(std::exchange(string.Size, 0))	{}


	String::String(const char8* text)
		: Length(CStringLength(text)),
		  Size(Length)
	{
		Data = new char[static_cast<size_t>(Size) + 1];
		memcpy(Data, text, static_cast<size_t>(Size) + 1);
	}

	String::String(const char8* text, int32 length)
		: Length(length),
		  Size(Length)
	{
		Data = new char[static_cast<size_t>(Size) + 1];
		memcpy(Data, text, static_cast<size_t>(Size));
		Data[Length] = '\0';
	}

	String::String(char8** charPtr)
		: Data(*charPtr),
		  Length(CStringLength(*charPtr)),
		  Size(Length)	{}

	String::String(char8** charPtr, int32 length)
		: Data(*charPtr),
		  Length(length),
		  Size(Length)	{}


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

	String& String::operator=(char8** charPtr)
	{
		return *this = String(charPtr);
	}


	bool String::operator==(const String& string) const
	{
		return strcmp(Data, string.Data) == 0;
	}


	void swap(String& left, String& right) noexcept
	{
		using std::swap;

		swap(left.Data, right.Data);
		swap(left.Length, right.Length);
		swap(left.Size, right.Size);
	}


	void String::Reverse() const
	{
		char8* forwardIterator  = Data;
		char8* backwardIterator = Data + Length - 1;
		while (forwardIterator < backwardIterator)
		{
			const char8 tmp   = *forwardIterator;
			*forwardIterator  = *backwardIterator;
			*backwardIterator = tmp;
			++forwardIterator;
			--backwardIterator;
		}
	}

	void String::Replace(char8 oldValue, char8 newValue)
	{
		char8* itr = Data - 1;
		while (*++itr != '\0')
		{
			if (*itr == oldValue)
				*itr = newValue;
		}
	}

	int32 String::FirstMatch(const char8* phrase) const
	{
		char8* itr               = Data - 1;
		const int32 phraseLength = CStringLength(phrase);
		while (*++itr != '\0' && itr - Data + phraseLength <= Length)
		{
			char8* compareIterator      = itr;
			const char8* phraseIterator = phrase;
			while (*compareIterator++ == *phraseIterator++)
			{
				if (*phraseIterator == '\0')
					return itr - Data;
			}
		}

		return -1;
	}

	void String::Remove(const char8* phrase)
	{
		int32 index;
		const int32 phraseLength = CStringLength(phrase);
		int32 numberOfRemoved    = 0;
		while ((index = FirstMatch(phrase)) != -1)
		{
			strcpy_s(Data + index, Length - index - phraseLength + 1, Data + index + phraseLength);
			++numberOfRemoved;
		}

		// TODO fix for child classes
		Length -= numberOfRemoved * phraseLength;
		Size = Length;
		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Size) + 1));
		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
		{
			Data = tmp;
		}
	}


	String String::Convert(int64 value, int32 base)
	{
		SString reverseString;
		reverseString.Reserve(20); // 20 - Maximum number of digits in 64 bit int

		// using ~ bitwise operator for getting rid of negative sign
		uint64 operationalValue = value > 0 ? value : 1 + static_cast<uint64>(~value);

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
			default:
			CL_ASSERT(false, "Only up to base 16 is covered!");
			}

			operationalValue /= base;
		} while (operationalValue);

		// Adding '-' if needed, at end of string before reversing
		if (value < 0)
			reverseString << "-";

		reverseString.Shrink();

		reverseString.Reverse();
		return reverseString;
	}

	String String::Convert(float64 value, int32 fractionLength)
	{
		char8* fString = d2fixed(value, static_cast<uint32>(fractionLength));
		String rString(fString);
		delete[] fString;
		return rString;
	}

	String String::Convert(void* address)
	{
		const String addressString = Convert(int64(address), 16);

		SString formattedAddress;
		formattedAddress.Reserve(18);
		formattedAddress << "0x";

		for (int32 i = 16 - addressString.GetLength(); i > 0; --i)
			formattedAddress << "0";

		return formattedAddress << addressString;
	}


	int32 String::CStringLength(const char8* string)
	{
		int32 length = 0;
		while (*string++)
			++length;

		return length;
	}


	BString::BString()
		: String()	{}

	BString::BString(const BString& string)
		: String(string) {}

	BString::BString(BString&& string) noexcept
		: String(static_cast<String&&>(string)) {}

	BString::BString(const char8* text)
		: String(text) {}

	BString::BString(const char8* text, int32 length)
		: String(text, length)	{}

	BString::BString(char8** charPtr)
		: String(charPtr) {}

	BString::BString(char8** charPtr, int32 length)
		: String(charPtr, length) {}

	BString& BString::operator=(const BString& string)
	{
		return *this = BString(string);
	}

	BString& BString::operator=(BString&& string) noexcept
	{
		swap(*this, string);
		return *this;
	}

	void swap(BString& left, BString& right) noexcept
	{
		using std::swap;

		swap(static_cast<String&>(left), static_cast<String&>(right));
	}

	void BString::SetSize(int32 size)
	{
		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(size) + 1));
		CL_ASSERT(tmp, "Failed to reallocate memory");

		if (tmp)
		{
			Size = size;
			Data = tmp;

			if (Length > Size)
			{
				Length = Size;

				Data[Length] = '\0';
			}
		}
	}

	void BString::Expand(uint32 size)
	{
		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Size) + size + 1));
		CL_ASSERT(tmp, "Failed to reallocate memory");

		if (tmp)
		{
			Size += size;
			Data = tmp;
		}
	}

	void BString::RecalculateLength()
	{
		Length = static_cast<int32>(strlen(Data));
	}


	SString::SString()
	{}

	SString::SString(const SString& string)
		: String(string.Data, string.Size)
	{
		Length = string.Length;
	}

	SString::SString(SString&& string) noexcept
		: String(static_cast<String&&>(string))	{}

	SString::SString(const char8* text)
	: String(text)	{}

	SString::SString(const char8* text, int32 length)
		: String(text, length)	{}

	SString::SString(char8** charPtr)
		: String(charPtr)	{}

	SString::SString(char8** charPtr, int32 length)
		: String(charPtr, length)	{}


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

		swap(static_cast<String&>(left), static_cast<String&>(right));
	}

	void SString::Reserve(int32 size)
	{
		Size      = size;
		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Size) + 1));

		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
			Data = tmp;
	}

	void SString::Shrink()
	{
		if (Length == Size)
			return;

		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Length) + 1));

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
			memcpy(Data + Length, string, static_cast<size_t>(size) + 1);
		else
		{
			Size = Length + size;

			auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Size) + 1));

			CL_ASSERT(tmp, "Failed to reallocate memory");
			if (tmp)
				Data = tmp;

			memcpy(Data + Length, string, static_cast<size_t>(size) + 1);
		}
		Length += size;
	}


	PString::PString()
	{}

	PString::PString(const PString& string)
		: String(string.Data, string.Size),
		  InsertIndex(string.InsertIndex),
		  InsertType(string.InsertType),
		  InsertSize(string.InsertSize)
	{
		Length = string.Length;
	}

	PString::PString(PString&& string) noexcept
		: String(static_cast<String&&>(string)),
		  InsertIndex(Move(string.InsertIndex)),
		  InsertType(Move(string.InsertType)),
		  InsertSize(Move(string.InsertSize))
	{}

	PString::PString(const char8* text ...)
	{
		std::vector<Ambiguous> parameters;
		std::vector<int32> insertSymbolSize;
		std::vector<int32> insertSymbolIndex;
		std::vector<int32> insertReplace;

		va_list args;
		va_start(args, text);

		int32 insert = false;

		int32 insertIndexOffset = 0;

		int32 srcSize = 0;

		int32 settingIndex = -1;

		for (int32 i = 0; text[i] != '\0'; ++i)
		{
			if (text[i] == 127)
			{
				insertSymbolIndex.push_back(++i);
				insertSymbolSize.push_back(1);
				insertReplace.push_back(-1);
				++srcSize;
				while (text[i] == 127)
				{
					--insertIndexOffset;
					++insertSymbolSize.back();
					++i;
					++srcSize;
				}
			}

			++srcSize;

			if (!insert)
			{
				switch (text[i])
				{
				case '%':
					// %% are escaped to %
					if (text[i + 1] == '%')
					{
						insertSymbolIndex.push_back(++i);
						insertSymbolSize.push_back(1);
						insertReplace.push_back(-1);
						--insertIndexOffset;
						++srcSize;
						break;
					}

					insertSymbolIndex.push_back(i);
					insertSymbolSize.push_back(2);
					InsertIndex.push_back(i + insertIndexOffset);
					InsertSize.push_back(0);

					parameters.push_back({});
					insert = true;
					break;
				default: ;
				}
			}
			else
			{
				if (settingIndex != -1 && text[i] != '}')
				{
					++insertSymbolSize.back();
					continue;
				}
				switch (text[i])
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					InsertSize.back() = InsertSize.back() * 10 + text[i] - '0';
					++insertSymbolSize.back();
					continue;

				case '{':
					++insertSymbolSize.back();
					settingIndex = i;
					continue;
				case '}':
					++insertSymbolSize.back();
					// TO-DO pass settings and context
					settingIndex = -1;
					continue;

				case 's':
					InsertType.push_back(text[i]);
					parameters.back().Char8Ptr = va_arg(args, char8*);
					break;
				case 'b':
				case 'd':
				case 'x':
					InsertType.push_back(text[i]);
					parameters.back().Int32 = va_arg(args, int32);
					break;
				case 'f':
					InsertType.push_back(text[i]);
					parameters.back().Float64 = va_arg(args, float64);
					break;
				case 'S':
					InsertType.push_back(text[i]);
					parameters.back().Char8Ptr = va_arg(args, char8*);
					InsertSize.back() += va_arg(args, int32);
					break;
				case 'B':
				case 'D':
				case 'X':
					InsertType.push_back(text[i]);
					parameters.back().Int32 = va_arg(args, int32);
					InsertSize.back() += va_arg(args, int32);
					break;
				case 'F':
					InsertType.push_back(text[i]);
					parameters.back().Float64 = va_arg(args, float64);
					InsertSize.back() += va_arg(args, int32);
					break;
				case 'R':
					InsertType.push_back(text[i]);
					parameters.back().VoidPtr = va_arg(args, void*);
					InsertSize.back() += static_cast<RString<>*>(parameters.back().VoidPtr)->Get()->GetLength();
					break;
				default:
				CL_ASSERT(false, "Unknow PString parameter type!");
				}

				// if current insert size is 0, assigns default size
				if (InsertSize.back() == 0)
					InsertSize.back() = TypeToSize(InsertType.back());

				insertReplace.push_back(static_cast<int32>(InsertIndex.size()) - 1);

				insertIndexOffset += InsertSize.back() - insertSymbolSize.back();
				insert       = false;
				settingIndex = -1;
			}
		}

		va_end(args);

		/*---------------------Size computation---------------------*/
		Size = CStringLength(text);

		for (int32 size : insertSymbolSize)
			Size -= size;

		for (int32 size : InsertSize)
			Size += size;
		/*----------------------------------------------------------*/

		delete[] Data;
		Data = new char8[static_cast<size_t>(Size) + 1];

		// Clearing memory with zero length space character
		memset(Data, 127, static_cast<size_t>(Size));

		// Adding escape character
		Data[Size] = '\0';


		auto insertSymbolIndexItr = insertSymbolIndex.begin();
		auto insertSymbolSizeItr  = insertSymbolSize.begin();
		auto insertReplaceItr     = insertReplace.begin();

		int32 dataOffset   = 0;
		int32 sourceOffset = 0;

		for (uint32 i = 0; i < insertSymbolIndex.size(); ++i)
		{
			const int copySize = *insertSymbolIndexItr - sourceOffset;
			memcpy(Data + dataOffset, text + sourceOffset, static_cast<size_t>(copySize));
			sourceOffset = *insertSymbolIndexItr++ + *insertSymbolSizeItr++;

			dataOffset += copySize;

			if (*insertReplaceItr != -1)
				dataOffset += InsertSize[*insertReplaceItr];

			++insertReplaceItr;
		}

		memcpy(Data + dataOffset, text + sourceOffset, static_cast<size_t>(srcSize) - sourceOffset);


		for (uint32 i = 0; i < InsertType.size(); ++i)
		{
			switch (InsertType[i])
			{
			case 's':
			case 'S':
				Set(i, parameters[i].Char8Ptr);
				break;
			case 'R':
				Set(i, *static_cast<RString<>*>(parameters[i].VoidPtr));
				break;
			case 'b':
			case 'B':
			case 'd':
			case 'D':
			case 'x':
			case 'X':
				Set(i, parameters[i].Int32);
				break;
			case 'f':
			case 'F':
				Set(i, parameters[i].Float64);
				break;
			default:
			CL_ASSERT(false, "Unknow PString parameter type!");
			}
		}

		RecalculateLength();
	}


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

		swap(static_cast<String&>(left), static_cast<String&>(right));
		swap(left.InsertIndex, right.InsertIndex);
		swap(left.InsertType, right.InsertType);
		swap(left.InsertSize, right.InsertSize);
	}


	String PString::ToString() const
	{
		const int32 length = GetLength();
		auto* newData      = new char8[static_cast<int64>(Length) + 1];

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
		return String(&newData, length);
	}

	int32 PString::TypeToSize(char8 type)
	{
		switch (type)
		{
		case 's':
		case 'S':
			return 16;
		case 'R':
			return -1;
		case 'b':
		case 'B':
			return 33;
		case 'd':
		case 'D':
			return 11;
		case 'x':
		case 'X':
			return 9;
		case 'f':
		case 'F':
			return 16;
		default:
		CL_ASSERT(false, "Unknow PString parameter type!");
		}

		return 0;
	}


	void PString::Set(int32 parameterIndex, const char8* text)
	{
		const int32 size = CStringLength(text);

		char8* index = Data + InsertIndex[parameterIndex];
		memset(index, 127, InsertSize[parameterIndex]);
		memcpy(index, text, size);

		RecalculateLength();
	}

	void PString::Set(int32 parameterIndex, const RString<>& string)
	{
		char8* index = Data + InsertIndex[parameterIndex];
		memset(index, 127, InsertSize[parameterIndex]);
		memcpy(index, string->CString(), string->GetLength());

		RecalculateLength();
	}

	void PString::Set(int32 parameterIndex, int32 value, int32 base)
	{
		char8* iterator = Data + InsertIndex[parameterIndex];
		memset(iterator, 127, InsertSize[parameterIndex]);

		iterator += static_cast<int64>(InsertSize[parameterIndex]) - 1;

		// using ~ bitwise operator for getting rid of negative sign
		uint64 operationalValue = value > 0 ? value : 1 + static_cast<uint64>(~value);

		do
		{
			switch (operationalValue % base)
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
			default:
			CL_ASSERT(false, "Only up to base 16 is covered!");
			}

			operationalValue /= base;
		} while (operationalValue);

		if (value < 0)
		{
			*iterator = '-';
		}

		RecalculateLength();
	}

	void PString::Set(int32 parameterIndex, int32 value)
	{
		switch (InsertType[parameterIndex])
		{
		case 'b':
		case 'B':
			Set(parameterIndex, value, 2);
			break;
		case 'd':
		case 'D':
			Set(parameterIndex, value, 10);
			break;
		case 'x':
		case 'X':
			Set(parameterIndex, value, 16);
			break;
		default:
		CL_ASSERT(false, "Unknow PString parameter type!");
		}
	}

	void PString::Set(int32 parameterIndex, float64 value)
	{
		char8* index = Data + InsertIndex[parameterIndex];
		memset(index, 127, static_cast<size_t>(InsertSize[parameterIndex]));

		d2fixed_buffered_n(value, 3, index);

		RecalculateLength();
	}


	void PString::RecalculateLength()
	{
		Length          = Size;
		char8* iterator = Data;
		do
		{
			if (*iterator == 127)
				--Length;
		} while (*iterator++);
	}
}

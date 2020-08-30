#include "pch.h"
#include "String.h"

#include "ryu/ryu.h"


namespace Coil
{
	String::String()
		: Data(nullptr),
		  Length(0),
		  Size(Length)
	{}

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
		  Size(std::exchange(string.Size, 0))
	{}


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
		  Size(Length)
	{}

	String::String(char8** charPtr, int32 length)
		: Data(*charPtr),
		  Length(length),
		  Size(Length)
	{}


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

	int32 String::Replace(const char8* oldPhrase, const char8* newPhrase)
	{
		const int32 oldPhraseLength = CStringLength(oldPhrase);
		const int32 newPhraseLength = CStringLength(newPhrase);
		const int32 sizeDifference  = newPhraseLength - oldPhraseLength;

		if (sizeDifference < 0)
			return ReplaceWithShorter(oldPhrase, newPhrase, oldPhraseLength, newPhraseLength, sizeDifference);

		if (sizeDifference == 0)
			return ReplaceWithEqual(oldPhrase, newPhrase, oldPhraseLength);

		return ReplaceWithLonger(oldPhrase, newPhrase, oldPhraseLength, newPhraseLength, sizeDifference);
	}

	int32 String::FirstMatch(const char8* phrase, int32 start, int32 end) const
	{
		char8* itr               = Data + start - 1;
		const int32 phraseLength = CStringLength(phrase);
		char8* itrEnd            = end < 0 ? Data + Length - phraseLength : Data + end - phraseLength;
		while (++itr <= itrEnd)
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

	int32 String::CountOccurrence(const char8* phrase) const
	{
		int32 index;
		int32 searchIndex        = 0;
		const int32 phraseLength = CStringLength(phrase);
		int32 numberOfOccurrence  = 0;
		while ((index = FirstMatch(phrase, searchIndex)) != -1)
		{
			searchIndex = index + phraseLength;
			++numberOfOccurrence;
		}

		return numberOfOccurrence;
	}

	void String::Remove(const char8* phrase)
	{
		int32 index;
		const int32 phraseLength = CStringLength(phrase);
		int32 numberOfRemoved    = 0;
		while ((index = FirstMatch(phrase)) != -1)
		{
			strcpy_s(Data + index, static_cast<size_t>(Length) - index - phraseLength + 1, Data + index + phraseLength);
			++numberOfRemoved;
		}

		// TODO fix for child classes
		Length -= numberOfRemoved * phraseLength;
		Size      = Length;
		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Size) + 1));
		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
		{
			Data = tmp;
		}
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
		auto* addressString = new char8[19]{ "0x0000000000000000" };

		const int32 size = IntRepresentationLength(int64(address), 16);

		Convert(addressString + 17 - size, size, int64(address), 16);

		return String(&addressString);
	}

	int32 String::ParseInt(const char8* source, int32 size)
	{
		int32 value     = 0;
		char8* iterator = const_cast<char8*>(source) - 1;
		char8* end      = const_cast<char8*>(source) + size;

		if (*source == '-')
			++iterator;

		while (++iterator != end)
		{
			int32 number = *iterator - '0';
			CL_ASSERT(0 <= number && number <= 9, "None numerical vaulue in source for ParseInt");
			value = value * 10 + number;
		}

		if (*source == '-')
			value = -value;

		return value;
	}


	int32 String::CStringLength(const char8* string)
	{
		int32 length = 0;
		while (*string++)
			++length;

		return length;
	}

	int32 String::ReplaceWithShorter(const char8* oldPhrase, const char8* newPhrase, int32 oldSize, int32 newSize, int32 difference)
	{
		int32 numberOfReplaced = 0;
		int32 searchIndex      = 0;
		int32 index;
		while ((index = FirstMatch(oldPhrase, searchIndex)) != -1)
		{
			memmove(Data + index + newSize, Data + index + oldSize, static_cast<size_t>(Length) - index - oldSize + 1);
			memcpy(Data + index, newPhrase, newSize);
			searchIndex = index + oldSize - 1;
			++numberOfReplaced;
		}

		Length += numberOfReplaced * difference;
		Size      = Length;
		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Size) + 1));
		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
		{
			Data = tmp;
		}

		return numberOfReplaced;
	}


	int32 String::ReplaceWithEqual(const char8* oldPhrase, const char8* newPhrase, int32 size)
	{
		int32 numberOfReplaced = 0;
		int32 searchIndex      = 0;
		int32 index;
		while ((index = FirstMatch(oldPhrase, searchIndex)) != -1)
		{
			memcpy(Data + index, newPhrase, size);
			searchIndex = index + size - 1;
			++numberOfReplaced;
		}

		return numberOfReplaced;
	}

	int32 String::ReplaceWithLonger(const char8* oldPhrase, const char8* newPhrase, int32 oldSize, int32 newSize, int32 difference)
	{
		int32 numberOfReplaced = CountOccurrence(oldPhrase);

		Length += numberOfReplaced * difference;
		Size      = Length;
		auto* tmp = static_cast<char8*>(realloc(Data, static_cast<size_t>(Size) + 1));
		CL_ASSERT(tmp, "Failed to reallocate memory");
		if (tmp)
		{
			Data = tmp;
		}

		int32 searchIndex = 0;
		int32 index;
		while ((index = FirstMatch(oldPhrase, searchIndex)) != -1)
		{
			memmove(Data + index + newSize, Data + index + oldSize, static_cast<size_t>(Length) - index - newSize + 1);
			memcpy(Data + index, newPhrase, newSize);
			searchIndex = index + newSize;
			++numberOfReplaced;
		}

		return numberOfReplaced;
	}
}

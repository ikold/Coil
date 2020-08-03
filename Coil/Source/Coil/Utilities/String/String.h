#pragma once


namespace Coil
{
	class String
	{
	public:
		/** Default Constructor */
		String();

		/** Copy Constructor */
		String(const String& string);

		/** Move Constructor */
		String(String&& string) noexcept;

		String(const char8* text);

		String(const char8* text, int32 length);

		explicit String(char8** charPtr);

		String(char8** charPtr, int32 length);


		/** Destructor */
		virtual ~String();


		/** Copy assignment operator */
		String& operator=(const String& string);

		/** Move assignment operator */
		String& operator=(String&& string) noexcept;

		String& operator=(const char8* string);

		String& operator=(char8** charPtr);


		bool operator==(const String& string) const;


		friend void swap(String& left, String& right) noexcept;


		[[nodiscard]] char8* CString() const { return Data; }

		[[nodiscard]] int32 GetLength() const { return Length; }

		[[nodiscard]] int32 GetSize() const { return Size; }


		void Reverse() const;

		void Replace(char8 oldValue, char8 newValue);

		int32 Replace(const char8* oldPhrase, const char8* newPhrase);

		int32 FirstMatch(const char8* phrase, int32 start = 0, int32 end = -1) const;

		int32 CountOccurence(const char8* phrase) const;

		void Remove(const char8* phrase);


		static String Convert(float64 value, int32 fractionLength);

		template<typename T>
		static char8* Convert(char8* destination, int32 size, T value, int32 base = 10);

		static String Convert(void* address);

		static int32 ParseInt(const char8* source, int32 size);

		template<typename T>
		static int32 IntRepresentationLength(T value, int32 base = 10)
		{
			int32 length = value < 0 ? 2 : 1;
			while (value /= base)
				++length;
			return length;
		}

		template<typename T>
		static int32 FloatRepresentationLength(T value)
		{
			int32 length = 0;

			if (value < 0)
			{
				value = -value;
				++length;
			}

			if (value < 1)
				return ++length;

			while (value >= 1)
			{
				++length;
				value /= 10;
			}

			return length;
		}

	protected:
		static int32 CStringLength(const char8* string);

		int32 ReplaceWithShorter(const char8* oldPhrase, const char8* newPhrase, int32 oldSize, int32 newSize, int32 difference);
		int32 ReplaceWithEqual(const char8* oldPhrase, const char8* newPhrase, int32 size);
		int32 ReplaceWithLonger(const char8* oldPhrase, const char8* newPhrase, int32 oldSize, int32 newSize, int32 difference);

	protected:
		char8* Data;
		int32 Length;
		int32 Size;
	};
}


namespace std
{
	template<>
	struct hash<Coil::String>
	{
		size_t operator()(Coil::String const& s) const noexcept
		{
			const size_t h1 = std::hash<char8*>{}(s.CString());
			const size_t h2 = std::hash<int32>{}(s.GetLength());
			return h1 ^ h2 << 1;
		}
	};
}


template<typename T>
char8* Coil::String::Convert(char8* destination, int32 size, T value, int32 base)
{
	int32 valueLength = IntRepresentationLength(value);

	// Using ~ bitwise operator for getting rid of negative sign
	uint64 operationalValue = value > 0 ? value : 1 + static_cast<uint64>(~value);

	while (--valueLength > size)
		operationalValue /= base;


	char8* inverseIterator = destination + valueLength;

	do
	{
		switch (operationalValue % base)
		{
		case 0:
			*inverseIterator = '0';
			break;
		case 1:
			*inverseIterator = '1';
			break;
		case 2:
			*inverseIterator = '2';
			break;
		case 3:
			*inverseIterator = '3';
			break;
		case 4:
			*inverseIterator = '4';
			break;
		case 5:
			*inverseIterator = '5';
			break;
		case 6:
			*inverseIterator = '6';
			break;
		case 7:
			*inverseIterator = '7';
			break;
		case 8:
			*inverseIterator = '8';
			break;
		case 9:
			*inverseIterator = '9';
			break;
		case 10:
			*inverseIterator = 'A';
			break;
		case 11:
			*inverseIterator = 'B';
			break;
		case 12:
			*inverseIterator = 'C';
			break;
		case 13:
			*inverseIterator = 'D';
			break;
		case 14:
			*inverseIterator = 'E';
			break;
		case 15:
			*inverseIterator = 'F';
			break;
		default:
		CL_ASSERT(false, "Only up to base 16 is covered!");
		}

		operationalValue /= base;
	} while (operationalValue && --inverseIterator >= destination);


	if (value < 0)
		*destination = '-';

	return destination + valueLength + 1;
}

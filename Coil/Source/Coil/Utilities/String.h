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


	class BString final : public String
	{
	public:
		/** Default Constructor */
		BString();

		/** Copy Constructor */
		BString(const BString& string);

		/** Move Constructor */
		BString(BString&& string) noexcept;

		BString(const char8* text);

		BString(const char8* text, int32 length);

		explicit BString(char8** charPtr);

		BString(char8** charPtr, int32 length);


		/** Destructor */
		~BString() override = default;


		/** Copy assignment operator */
		BString& operator=(const BString& string);

		/** Move assignment operator */
		BString& operator=(BString&& string) noexcept;


		friend void swap(BString& left, BString& right) noexcept;

		void SetSize(int32 size);

		void SetLength(int32 length) { Length = length; }

		void Expand(uint32 size);

		void RecalculateLength();

		[[nodiscard]] char8* GetBuffer() const { return Data; }
	};


	class SString final : public String
	{
	public:
		/** Default Constructor */
		SString();

		/** Copy Constructor */
		SString(const SString& string);

		/** Move Constructor */
		SString(SString&& string) noexcept;

		SString(const char8* text);

		SString(const char8* text, int32 length);

		explicit SString(char8** charPtr);

		SString(char8** charPtr, int32 length);


		/** Destructor */
		~SString() override = default;


		/** Copy assignment operator */
		SString& operator=(const SString& string);

		/** Move assignment operator */
		SString& operator=(SString&& string) noexcept;


		friend void swap(SString& left, SString& right) noexcept;


		void Reserve(int32 size);

		void Shrink();


		SString& operator<<(const char8* string);
		SString& operator<<(const String& string);

		//SString& operator<<(int8 value) { return *this << Convert(static_cast<int64>(value)); }
		//SString& operator<<(uint8 value) { return *this << Convert(static_cast<int64>(value)); }
		//SString& operator<<(int16 value) { return *this << Convert(static_cast<int64>(value)); }
		//SString& operator<<(uint16 value) { return *this << Convert(static_cast<int64>(value)); }
		//SString& operator<<(int32 value) { return *this << Convert(static_cast<int64>(value)); }
		//SString& operator<<(uint32 value) { return *this << Convert(static_cast<int64>(value)); }
		//SString& operator<<(int64 value) { return *this << Convert(static_cast<int64>(value)); }

		SString& operator<<(float32 value) { return *this << Convert(value, 3); }
		SString& operator<<(float64 value) { return *this << Convert(value, 3); }

		SString& operator<<(void* address) { return *this << Convert(address); }


		void SetFractionLength(int32 value) { FractionLength = value; }

	private:
		void Append(const char8* string, int32 size);

		int32 FractionLength = 3;
	};


	template<class TString = String>
	class RString
	{
	public:
		/** Default Constructor */
		RString()
			: StringPointer(nullptr),
			  Counter(nullptr) {}

		/** Copy Constructor */
		RString(const RString& rString)
			: StringPointer(rString.StringPointer),
			  Counter(rString.Counter)
		{
			++*Counter;
		}

		/** Move Constructor */
		RString(RString&& rString) noexcept
			: StringPointer(std::exchange(rString.StringPointer, nullptr)),
			  Counter(std::exchange(rString.Counter, nullptr)) {}

		RString(const char8* text)
			: StringPointer(new TString(text)),
			  Counter(new int32(1)) {}

		RString(const TString& string)
			: StringPointer(new TString(string)),
			  Counter(new int32(1)) {}

		RString(TString&& string)
			: StringPointer(new TString(Move(string))),
			  Counter(new int32(1)) {}


		/** Destructor */
		~RString()
		{
			DereferenceString();
		}


		/** Copy assignment operator */
		RString& operator=(const RString& string)
		{
			return *this = RString(string);
		}

		/** Move assignment operator */
		RString& operator=(RString&& string) noexcept
		{
			swap(*this, string);
			return *this;
		}

		RString& operator=(const char8* string)
		{
			swap(*this, RString(string));
			return *this;
		}

		RString& operator=(const TString& string)
		{
			swap(*this, RString(string));
			return *this;
		}

		RString& operator=(TString&& string)
		{
			swap(*this, RString(Move(string)));
			return *this;
		}


		friend void swap(RString& left, RString& right) noexcept
		{
			using std::swap;

			swap(left.StringPointer, right.StringPointer);
			swap(left.Counter, right.Counter);
		}


		TString* operator->() const { return Get(); }

		TString& operator*() const { return *Get(); }


		bool operator==(const RString<TString>& string) const
		{
			return strcmp(Get()->CString(), string.Get()->CString()) == 0;
		}


		template<class TCast>
		operator RString<TCast>() const { return *(reinterpret_cast<const RString<TCast>*>(this)); }


		[[nodiscard]] TString* Get() const { return static_cast<TString*>(StringPointer); }


		[[nodiscard]] RString Copy() const
		{
			return RString(*Get());
		}

		static RString Copy(const RString& string)
		{
			return string.Copy();
		}

	protected:
		void DereferenceString() const
		{
			if (Counter && -- *Counter == 0)
			{
				delete StringPointer;
				delete Counter;
			}
		}

	protected:
		String* StringPointer;
		int32* Counter;
	};


	class PString final : public String
	{
	public:
		struct Settings
		{
			int32 DecimalPoint = -1;
		};


	public:
		/** Default Constructor */
		PString() = default;

		/** Copy Constructor */
		PString(const PString& string);

		/** Move Constructor */
		PString(PString&& string) noexcept;

		PString(const char8* text...);


		/** Destructor */
		~PString() override = default;


		/** Copy assignment operator */
		PString& operator=(const PString& string);

		/** Move assignment operator */
		PString& operator=(PString&& string) noexcept;


		friend void swap(PString& left, PString& right) noexcept;


		[[nodiscard]] String ToString() const;


		static int32 TypeToSize(char8 type);


		[[nodiscard]] int32 GetParameterSize(int32 insertIndex) const { return InsertSize[insertIndex]; }

		void Set(int32 parameterIndex, const char8* text);
		void Set(int32 parameterIndex, const RString<>& string);

		void Set(int32 parameterIndex, int64 value);
		void Set(int32 parameterIndex, int64 value, int32 base);
		void Set(int32 parameterIndex, int32 value) { Set(parameterIndex, static_cast<int64>(value)); }
		void Set(int32 parameterIndex, int32 value, int32 base) { Set(parameterIndex, static_cast<int64>(value), base); }
		void Set(int32 parameterIndex, uint32 value) { Set(parameterIndex, static_cast<int64>(value)); }
		void Set(int32 parameterIndex, uint32 value, int32 base) { Set(parameterIndex, static_cast<int64>(value), base); }
		void Set(int32 parameterIndex, uint64 value) { Set(parameterIndex, static_cast<int64>(value)); }
		void Set(int32 parameterIndex, uint64 value, int32 base) { Set(parameterIndex, static_cast<int64>(value), base); }

		void Set(int32 parameterIndex, float64 value);

	private:
		void RecalculateLength();

		static Settings ParseSettings(const char8* source, int32 size);

	private:
		/** Points at the beginning of insert padding */
		std::vector<int32> InsertIndex{};
		/** Uses characters to denote the types e.g. 'd' for int32 */
		std::vector<char8> InsertType{};
		/** Size of insert padding */
		std::vector<int32> InsertSize{};
		/** Settings of insert */
		std::vector<Settings> InsertSettings{};
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


	template<>
	struct hash<Coil::SString>
	{
		size_t operator()(Coil::SString const& s) const noexcept
		{
			const size_t h1 = std::hash<char8*>{}(s.CString());
			const size_t h2 = std::hash<int32>{}(s.GetLength());
			const size_t h3 = std::hash<int32>{}(s.GetSize());
			return h1 ^ h2 << 1 ^ h3 << 2;
		}
	};


	template<>
	struct hash<Coil::PString>
	{
		size_t operator()(Coil::PString const& s) const noexcept
		{
			const size_t h1 = std::hash<char8*>{}(s.CString());
			const size_t h2 = std::hash<int32>{}(s.GetLength());
			const size_t h3 = std::hash<int32>{}(s.GetSize());
			return h1 ^ h2 << 1 ^ h3 << 2;
		}
	};


	template<class TString>
	struct hash<Coil::RString<TString>>
	{
		size_t operator()(Coil::RString<TString> const& s) const noexcept
		{
			const size_t h1 = std::hash<char8*>{}(s->CString());
			const size_t h2 = std::hash<int32>{}(s->GetLength());
			const size_t h3 = std::hash<int32>{}(s->GetSize());
			return h1 ^ h2 << 1 ^ h3 << 2;
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

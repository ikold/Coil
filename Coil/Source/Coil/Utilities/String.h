#pragma once

#include "Coil/Core.h"

namespace Coil
{
	class COIL_API String
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

		String(char8** charPtr);

		String(char8** charPtr, int32 length);


		/** Destructor */
		virtual ~String();


		/** Copy assignment operator */
		String& operator=(const String& string);

		/** Move assignment operator */
		String& operator=(String&& string) noexcept;

		String& operator=(const char8* string);


		friend void swap(String& left, String& right) noexcept;


		inline char8* CString() const { return Data; }

		inline int32 GetLength() const { return Length; }


		void Reverse();


		static String Convert(float64 value, int fractionLength);

		static String Convert(uint64 value, int32 base = 10);

		static String Convert(void* address);

	protected:
		static int32 CStringLength(const char8* string);

	protected:
		char8* Data;
		int32 Length;
	};


	class COIL_API SString : public String
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


		/** Destructor */
		~SString();


		/** Copy assignment operator */
		SString& operator=(const SString& string);

		/** Move assignment operator */
		SString& operator=(SString&& string) noexcept;


		friend void swap(SString& left, SString& right) noexcept;


		void Reserve(int32 size);

		void Shrink();


		SString& operator<<(const char8* string);
		SString& operator<<(const String& string);

		SString& operator<<(int8 value) { return *this << String::Convert(value); };
		SString& operator<<(uint8 value) { return *this << String::Convert(value); };
		SString& operator<<(int16 value) { return *this << String::Convert(value); };
		SString& operator<<(uint16 value) { return *this << String::Convert(value); };
		SString& operator<<(int32 value) { return *this << String::Convert(value); };
		SString& operator<<(uint32 value) { return *this << String::Convert(value); };
		SString& operator<<(int64 value) { return *this << String::Convert(value); };
		SString& operator<<(uint64 value) { return *this << String::Convert(value); };

		SString& operator<<(float32 value) { return *this << String::Convert(value, 3); };
		SString& operator<<(float64 value) { return *this << String::Convert(value, 3); };

		SString& operator<<(void* address) { return *this << String::Convert(address); };


		void SetFractionLength(int32 value) { FractionLength = value; }

	private:
		void Append(const char8* string, int32 size);

	private:
		int32 Size;
		int32 FractionLength = 3;
	};


	class COIL_API PString : public String
	{
	public:
		/** Default Constructor */
		PString();

		/** Copy Constructor */
		PString(const PString& string);

		/** Move Constructor */
		PString(PString&& string) noexcept;

		PString(char8* text...);


		/** Destructor */
		~PString();


		/** Copy assignment operator */
		PString& operator=(const PString& string);

		/** Move assignment operator */
		PString& operator=(PString&& string) noexcept;


		friend void swap(PString& left, PString& right) noexcept;

		String ToString() const;


		static int32 TypeToSize(char8 type);


		inline int32 GetParameterSize(int32 insertIndex) const { return InsertSize[insertIndex]; };

		void Set(int32 insertIndex, char8* text);
		void Set(int32 insertIndex, int32 value);
		void Set(int32 insertIndex, int32 value, int32 base);
		void Set(int32 insertIndex, float64 value);

	private:
		void RecalculateLength();

	private:
		int32 Size;
		// points at the beginning of insert padding
		std::vector<int32> InsertIndex;
		// uses characters to denote the types e.g. 'd' for int32
		std::vector<char8> InsertType;
		// size of insert padding
		std::vector<int32> InsertSize;
	};


	template<class TString = String>
	class COIL_API RString
	{
	public:
		/** Default Constructor */
		RString()
			: StringPointer(nullptr),
			Counter(nullptr)
		{}

		/** Copy Constructor */
		RString(const RString& rString)
		{
			StringPointer = rString.StringPointer;
			Counter = rString.Counter;
			++(*Counter);
		}

		/** Move Constructor */
		RString(RString&& rString) noexcept
			: StringPointer(std::exchange(rString.StringPointer, nullptr)),
			Counter(std::exchange(rString.Counter, nullptr))
		{}

		RString(const char8* text)
		{
			StringPointer = new String(text);
			Counter = new int32(1);
		}

		RString(const TString& string)
		{
			StringPointer = new TString(string);
			Counter = new int32(1);
		}

		RString(TString&& string)
			: StringPointer(new TString(std::move(string))),
			Counter(new int32(1))
		{}


		/** Destructor */
		~RString()
		{
			DerefencString();
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


		friend void swap(RString& left, RString& right) noexcept
		{
			using std::swap;

			swap(left.StringPointer, right.StringPointer);
			swap(left.Counter, right.Counter);
		}


		TString* operator->() const { return Get(); }

		inline TString& operator*() const { return *Get(); }


		template<class TCast>
		operator RString<TCast>() const { return (RString<TCast>&) * this; }


		inline TString* Get() const { return static_cast<TString*>(StringPointer); }


		inline RString Copy() const
		{
			return RString(*Get());
		}

		static inline RString Copy(const RString& string)
		{
			return string.Copy();
		}

	protected:
		void DerefencString()
		{
			if (Counter && --(*Counter) == 0)
			{
				delete StringPointer;
				delete Counter;
			}
		}

	protected:
		String* StringPointer;
		int32* Counter;
	};
}
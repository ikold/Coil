#pragma once

#include "Coil/Core.h"


namespace Coil
{
	class SString;
	class PString;

	class COIL_API String
	{
	public:
		String();
		String(const char8* text);
		String(const String& string);
		String(String&& string) noexcept;

		String(const PString& string);

		~String();

		inline char8* CString() const { return Data; }

		inline int32 GetLength() const { return Length; }

		void Reverse();

		String& operator=(const char8* str);
		String& operator=(const String& str);

		String& operator=(String&& str) noexcept;


		static inline String Convert(int8 value, int base = 10)		{ return Convert<int8>(value, base); };
		static inline String Convert(uint8 value, int base = 10)	{ return Convert<uint8>(value, base); };
		static inline String Convert(int16 value, int base = 10)	{ return Convert<int16>(value, base); };
		static inline String Convert(uint16 value, int base = 10)	{ return Convert<uint16>(value, base); };
		static inline String Convert(int32 value, int base = 10)	{ return Convert<int32>(value, base); };
		static inline String Convert(uint32 value, int base = 10)	{ return Convert<uint32>(value, base); };
		static inline String Convert(int64 value, int base = 10)	{ return Convert<int64>(value, base); };
		static inline String Convert(uint64 value, int base = 10)	{ return Convert<uint64>(value, base); };

		static inline String Convert(float32 value, int fractionLength);
		static String Convert(float64 value, int fractionLength);

		static String Convert(void* address);

		friend void swap(String& left, String& right) noexcept
		{
			using std::swap;

			swap(left.Data, right.Data);
			swap(left.Length, right.Length);
		}

	protected:
		template<typename T>
		static typename std::enable_if<std::is_integral<T>::value, String>::type
		Convert(T value, int32 base)
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

			// Adding '-' if needed, at end of string before revesing
			if (value < 0)
			{
				reverseString << "-";
			}

			reverseString.Shrink();

			reverseString.Reverse();
			return reverseString;
		}

		static int32 CStringLength(const char8* str);

	protected:
		char8* Data;

		int32 Length;
	};

	class COIL_API RString
	{
	public:
		RString();

		RString(const RString& rString);
		RString(const char8* text);

		template<class T, TDerives(String)>
		RString(const T& string)
		{
			StringPointer = new T(string);
			Counter = new int32(1);
		}

		RString(RString&& rString);

		template<class T, TDerives(String)>
		RString(T&& string)
			: StringPointer(new T(std::move(string))),
			Counter(new int32(1))
		{}

		~RString();

		RString& operator=(const char8* str);
		RString& operator=(const RString& str);

		template<class T, TDerives(String)>
		RString& operator=(const T& str)
		{
			swap(*this, RString(str));
			return *this;
		}
		
		RString& operator=(RString&& str);

		template<class T, TDerives(String)>
		RString& operator=(T&& str)
		{
			swap(*this, RString(str));
			return *this;
		}

		String* operator->() const { return Get<String>(); }
		inline String& operator*() const { return *Get<String>(); }

		inline RString Copy();
		static inline RString Copy(const RString& str);

		template<class T, TDerives(String)>
		inline T* Get() const { return static_cast<T*>(StringPointer); }

		friend void swap(RString& left, RString& right) noexcept
		{
			using std::swap;

			swap(left.StringPointer, right.StringPointer);
			swap(left.Counter, right.Counter);
		}

	private:
		void DerefencString();

	private:
		String* StringPointer;
		int32* Counter;
	};

	class COIL_API SString : public String
	{
	public:
		SString();
		SString(const char8* text);
		~SString();

		void Reserve(int32 size);

		void Shrink();


		SString& operator<<(const char8* str);
		SString& operator<<(const String& str);
		SString& operator<<(const RString& str);
		SString& operator<<(const SString& str);

		SString& operator<<(int8 value)		{ return *this << String::Convert(value); };
		SString& operator<<(uint8 value)	{ return *this << String::Convert(value); };
		SString& operator<<(int16 value)	{ return *this << String::Convert(value); };
		SString& operator<<(uint16 value)	{ return *this << String::Convert(value); };
		SString& operator<<(int32 value)	{ return *this << String::Convert(value); };
		SString& operator<<(uint32 value)	{ return *this << String::Convert(value); };
		SString& operator<<(int64 value)	{ return *this << String::Convert(value); };
		SString& operator<<(uint64 value)	{ return *this << String::Convert(value); };

		SString& operator<<(float32 value) { return *this << String::Convert(value, 3); };
		SString& operator<<(float64 value) { return *this << String::Convert(value, 3); };

		SString& operator<<(void* address) { return *this << String::Convert(address); };


		void SetFractionLength(int32 value) { FractionLength = value; }

	private:
		void Append(const char8* str, int32 size);

	private:
		int32 Size;
		int32 FractionLength = 3;
	};

	class COIL_API PString : public String
	{
	public:
		PString(char8* text...);

		int32 TypeToSize(char8 type) const;

		void Set(int32 elementIndex, char8* text);
		void Set(int32 elementIndex, int32 value);
		void Set(int32 elementIndex, float64 value);

	private:
		void CalculateLength();

	private:
		int32 Size;
		// points at the end of the space
		std::vector<int32> InsertIndex;
		// uses char to denote the types e.g. 'd' for int64
		std::vector<char8> InsertType;
	};
}
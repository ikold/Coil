#pragma once

#include "Coil/Core.h"


namespace Coil
{
	class COIL_API String
	{
	public:
		String();
		String(const char8* text...);
		String(const String& string);
		~String();

		inline char8* CString() const { return Data; }

		inline int32 GetLength() const { return Length; }

		void Reverse();

		String& operator=(const char8* str);
		String& operator=(const String& str);


		static inline String Convert(int8 value, int base = 10)		{ return Convert<int8>(value, base); };
		static inline String Convert(uint8 value, int base = 10)	{ return Convert<uint8>(value, base); };
		static inline String Convert(int16 value, int base = 10)	{ return Convert<int16>(value, base); };
		static inline String Convert(uint16 value, int base = 10)	{ return Convert<uint16>(value, base); };
		static inline String Convert(int32 value, int base = 10)	{ return Convert<int32>(value, base); };
		static inline String Convert(uint32 value, int base = 10)	{ return Convert<uint32>(value, base); };
		static inline String Convert(int64 value, int base = 10)	{ return Convert<int64>(value, base); };
		static inline String Convert(uint64 value, int base = 10)	{ return Convert<uint64>(value, base); };

		static inline String Convert(float32 value, int fractionLength);
		static inline String Convert(float64 value, int fractionLength);

		static String Convert(void* address);

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
		RString(const String& string);
		RString(const RString& rString);
		RString(const char8* text);
		~RString();

		String* operator->() const { return Get(); }

		RString& operator=(const char8* str);
		RString& operator=(const String& str);
		RString& operator=(const RString& str);

		inline String& operator*() const { return *Get(); }

		static inline RString Copy(const RString& str);

	private:
		inline String* Get() const { return StringPointer; }

		void DerefencString();

	private:
		String* StringPointer;
		int* Counter;
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
}
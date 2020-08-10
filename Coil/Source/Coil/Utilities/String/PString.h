#pragma once
#include "String.h"
#include "RString.h"


namespace Coil
{
	/**
	 * @brief Parameterized String
	 *
	 * @note Parameter Types:\n
	 * b - binary int32\n
	 * d - decimal int32\n
	 * x - hexadecimal int32\n
	 * f - float64\n
	 * s - zero-terminated char8*\n
	 * R - RString<>\n
	 *
	 * @note Constructor accepts only written types, but Set() methods are defined for wide range of types (int64, float32 etc.)
	 * 
	 * @note PString uses zero-width spaces for reserving size for parameters, for font to be compatible it has to have zero-width space on 127 character (return - normally unused in displaying text)
	 * @note If used with unsupported font, ToString() is provided to create copy without return character (127 character used for zero-width space)\n
	 * 
	 * @note Parameters are denoted by prefixing it with % (e.g. "number %d")\n
	 * 
	 * @note Size of the parameter (how much space is reserved in the PString) can be provided by inserting number between % and denoting character (e.g. "number %4d")\n
	 * Except for R all of the parameters have upper case equivalent that accepts size as second parameter (after the value of the parameter), if size is defined in source string end result is sum of the two values (e.g. PString("number %2D" -10, 2) is one parameter of size 4 and value of -10 and would be displayed as "number 10")\n
	 * 
	 * @note Additional settings can be provided in the source string by inserting them between % and denoting character in the curve brackets (e.g. "number %4{.2}d"), options should not split size number (treated as undefined behavior)
	 *
	 * @todo Allow resizing (reallocation) of reserved space for the parameters
	 */
	class PString final : public String
	{
	public:
		/**
		 * @brief Settings that can be defined in the curved brackets
		 *
		 * param	DecimalPoint	{.[digits_num]}		Used for determining number of digits in common fraction
		 *
		 * @note Unused setting for given type are ignored even if defined, but still stored
		 * @note If DecimalPoint is used for integer types, fraction digits are obtained inserting dot (e.g. PString("pi = %{.2}d", 314) would be displayed as "pi = 3.14)
		 */
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


		/**
		 * @brief Creates basic String copy without zero-width characters
		 *
		 * @return		String copy of the PString
		 */
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
	struct hash<Coil::PString>
	{
		size_t operator()(Coil::PString const& s) const noexcept
		{
			// TODO use String hash by casting to base String
			const size_t h1 = std::hash<char8*>{}(s.CString());
			const size_t h2 = std::hash<int32>{}(s.GetLength());
			const size_t h3 = std::hash<int32>{}(s.GetSize());
			return h1 ^ h2 << 1 ^ h3 << 2;
		}
	};
}

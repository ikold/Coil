#pragma once
#include "String.h"
#include "RString.h"


namespace Coil
{
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
}

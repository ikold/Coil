#pragma once
#include "String.h"


namespace Coil
{
	/**
	 * @brief Specialize reference counter for strings
	 *
	 * @tparam[in]	TString		String type
	 *
	 * @todo Write a documentation
	 */
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

		void Detach()
		{
			if (*Counter == 1)
				return;

			*this = Copy();
		}

	protected:
		void DereferenceString() const
		{
			if (Counter && --*Counter == 0)
			{
				delete StringPointer;
				delete Counter;
			}
		}

	private:
		String* StringPointer;
		int32* Counter;
	};
}


namespace std
{
	template<class TString>
	struct hash<Coil::RString<TString>>
	{
		size_t operator()(Coil::RString<TString> const& string) const noexcept
		{
			return std::hash<Coil::String>{}(*string);
		}
	};
}

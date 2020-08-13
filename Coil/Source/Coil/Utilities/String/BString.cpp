#include "pch.h"
#include "BString.h"


namespace Coil
{
	BString::BString()
		: String() {}

	BString::BString(const BString& string)
		: String(string) {}

	BString::BString(BString&& string) noexcept
		: String(static_cast<String&&>(string)) {}

	BString::BString(const char8* text)
		: String(text) {}

	BString::BString(const char8* text, int32 length)
		: String(text, length) {}

	BString::BString(int32 length)
		: String(new char8[length]{}, length) {}

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
}

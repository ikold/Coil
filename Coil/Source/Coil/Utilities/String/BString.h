#pragma once
#include "String.h"


namespace Coil
{
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
}

#pragma once
#include "String.h"


namespace Coil
{
	/**
	 * @brief Specialize buffer string
	 *
	 * @note Should use SetSize(size) and Expand(size) if current storage Size is not sufficient
	 * @note After changing Data RecalculateLength() has to be called or Length has to be manually set by calling SetLength(length) 
	 */
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

		/**
		 * @see String(char**)
		 */
		explicit BString(char8** charPtr);

		BString(char8** charPtr, int32 length);


		/** Destructor */
		~BString() override = default;


		/** Copy assignment operator */
		BString& operator=(const BString& string);

		/** Move assignment operator */
		BString& operator=(BString&& string) noexcept;


		friend void swap(BString& left, BString& right) noexcept;

		/**
		 * @brief Reallocates Data with the new Size
		 *
		 * @param[in]	size	New size in bytes excluding zero-terminated character
		 *
		 * @note Old data is copied
		 * @note For smaller new size, zero-terminated character is set at the end of the BString
		 * @note For bigger new size, no data is set to previously unused region
		 */
		void SetSize(int32 size);

		/**
		 * @brief Reallocates Data with Size increased by passed value
		 *
		 * @param[in]	size	Number of bytes to expand BString
		 * 
		 * @see SetSize(size)
		 */
		void Expand(uint32 size);
		
		void SetLength(int32 length) { Length = length; }

		/**
		 * @brief Recalculates BString Length base on number of characters before zero-terminated character
		 */
		void RecalculateLength();

		[[nodiscard]] char8* GetBuffer() const { return Data; }
	};
}

#pragma once


namespace Coil
{
	class Binary
	{
	public:
		Binary();

		Binary(const Binary& rhs);

		Binary(Binary&& rhs) noexcept;

		Binary(byte** data, int32 size);

		~Binary();

		Binary& operator=(const Binary& rhs);

		Binary& operator=(Binary&& rhs) noexcept;


		friend void swap(Binary& left, Binary& right) noexcept;


		[[nodiscard]] byte* GetData() const { return Data; }
		[[nodiscard]] int32 GetSize() const { return Size; }

	private:
		byte* Data;
		int32 Size;
	};

	
	class File
	{
	public:
		File() = delete;

		[[nodiscard]] static RString<> Load(const RString<>& filePath);
		
		[[nodiscard]] static Binary LoadBinary(const RString<>& filePath);
	};
}

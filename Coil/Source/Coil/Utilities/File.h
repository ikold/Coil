#pragma once


namespace Coil
{
	class Binary
	{
	public:
		Binary();
		Binary(byte** data, int32 size);

		~Binary();

		[[nodiscard]] byte* GetDate() const { return Data; }
		[[nodiscard]] int32 GetSize() const { return Size; }

	private:
		byte* Data;
		int32 Size;
	};

	
	class File
	{
	public:
		File() = delete;

		[[nodiscard]] static RString<String> Load(const RString<String>& filePath);
		
		[[nodiscard]] static Binary LoadBinary(const RString<String>& filePath);
	};
}

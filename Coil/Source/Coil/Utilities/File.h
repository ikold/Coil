#pragma once


namespace Coil
{
	class File
	{
	public:
		File() = delete;

		[[nodiscard]] static RString<String> Load(RString<String> filePath);
	};
}

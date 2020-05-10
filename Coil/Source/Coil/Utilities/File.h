#pragma once

#include "Coil/Core.h"


namespace Coil
{
	class File
	{
	public:
		File() = delete;

		static RString<String> Load(RString<String> filePath);
	};
}

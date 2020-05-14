#include "pch.h"
#include "File.h"

#include  <fstream>


namespace Coil
{
	RString<String> File::Load(RString<String> filePath)
	{
		std::ifstream fileStream(filePath->CString());

		if (fileStream)
		{
			fileStream.ignore(std::numeric_limits<std::streamsize>::max());
			std::streamsize length = fileStream.gcount();
			fileStream.clear();
			fileStream.seekg(0, std::ifstream::beg);

			auto* buffer = new char8[static_cast<size_t>(length) + 1];
			buffer[length] = '\0';

			
			String fileContent(&buffer, length); //In case of failed read String deletes buffer

			fileStream.read(buffer, length);

			return fileContent;
		}

		CL_CORE_ASSERT(false, PString("Loading of '%S' failed!", filePath->CString(), filePath->GetSize()));

		return String("");
	}
}

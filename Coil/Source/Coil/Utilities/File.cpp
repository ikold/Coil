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
			fileStream.seekg(0, std::ifstream::end);
			const int32 length = fileStream.tellg();
			fileStream.seekg(0, std::ifstream::beg);

			auto* buffer = new char8[length + 1]();
			
			String fileContent(&buffer, length); //In case of failed read String deletes buffer
			
			fileStream.read(buffer, length);

			return fileContent;
		}

		CL_CORE_ASSERT(false, PString("Loading of '%S' failed!", filePath->CString(), filePath->GetSize()));
		
		return String("");
	}
}

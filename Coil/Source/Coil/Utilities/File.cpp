#include "pch.h"
#include "File.h"

#include <fstream>
#include <iostream>


namespace Coil
{
	Binary::Binary()
		: Data(nullptr),
		  Size(0)
	{}

	Binary::Binary(byte** data, int32 size)
		: Data(*data),
		  Size(size)
	{}

	Binary::~Binary()
	{
		delete[] Data;
	}


	RString<> File::Load(const RString<>& filePath)
	{
		std::ifstream fileStream(filePath->CString());

		if (fileStream)
		{
			fileStream.ignore(std::numeric_limits<std::streamsize>::max());
			const std::streamsize length = fileStream.gcount();
			fileStream.clear();
			fileStream.seekg(0, std::ifstream::beg);

			auto* buffer   = new char8[static_cast<size_t>(length) + 1];
			buffer[length] = '\0';


			String fileContent(&buffer, length); //In case of failed read String deletes buffer

			fileStream.read(buffer, length);

			return fileContent;
		}

		CL_CORE_ASSERT(false, PString("Loading of '%S' failed!", filePath->CString(), filePath->GetSize()));

		return String("");
	}

	Binary File::LoadBinary(const RString<>& filePath)
	{
		std::ifstream fileStream(filePath->CString(), std::ios::in | std::ios::binary | std::ios::ate);

		if (fileStream)
		{
			fileStream.ignore(std::numeric_limits<std::streamsize>::max());
			const std::streamsize length = fileStream.gcount();
			fileStream.clear();
			fileStream.seekg(0, std::ifstream::beg);

			auto* buffer = new byte[static_cast<size_t>(length) + 1];


			Binary fileContent(&buffer, length); //In case of failed read Binary deletes buffer

			fileStream.read(reinterpret_cast<char*>(buffer), length);

			return fileContent;
		}

		CL_CORE_ASSERT(false, PString("Loading of '%S' failed!", filePath->CString(), filePath->GetSize()));

		return Binary();
	}
}

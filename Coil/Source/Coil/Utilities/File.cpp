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

	Binary::Binary(const Binary& rhs)
		: Size(rhs.Size)
	{
		Data = new byte[Size];
		memcpy(Data, rhs.Data, Size);
	}

	Binary::Binary(Binary&& rhs) noexcept
		: Data(std::exchange(rhs.Data, nullptr)),
		  Size(std::exchange(rhs.Size, 0))
	{}


	Binary::Binary(byte* * data, int32 size)
		: Data(*data),
		  Size(size)
	{}


	Binary::~Binary()
	{
		delete[] Data;
	}

	Binary& Binary::operator=(const Binary& rhs)
	{
		return *this = Binary(rhs);
	}

	Binary& Binary::operator=(Binary&& rhs) noexcept
	{
		swap(*this, rhs);
		return *this;
	}


	void swap(Binary& left, Binary& right) noexcept
	{
		using std::swap;

		swap(left.Data, right.Data);
		swap(left.Size, right.Size);
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


			String fileContent(&buffer, static_cast<int32>(length)); //In case of failed read String deletes buffer

			fileStream.read(buffer, length);

			return fileContent;
		}

		CL_CORE_ASSERT(false, PString("Loading of '%R' failed!", &filePath));

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


			Binary fileContent(&buffer, static_cast<int32>(length)); //In case of failed read Binary deletes buffer

			fileStream.read(reinterpret_cast<char*>(buffer), length);

			return fileContent;
		}

		CL_CORE_ASSERT(false, PString("Loading of '%S' failed!", &filePath));

		return Binary();
	}
}

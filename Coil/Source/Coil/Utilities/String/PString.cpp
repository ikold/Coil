﻿#include "pch.h"
#include "PString.h"
#include "ryu/ryu.h"


namespace Coil
{
	PString::PString(const PString& string)
		: String(string.Data, string.Size),
		InsertIndex(string.InsertIndex),
		InsertType(string.InsertType),
		InsertSize(string.InsertSize),
		InsertSettings(string.InsertSettings)
	{
		Length = string.Length;
	}

	PString::PString(PString&& string) noexcept
		: String(static_cast<String&&>(string)),
		InsertIndex(Move(string.InsertIndex)),
		InsertType(Move(string.InsertType)),
		InsertSize(Move(string.InsertSize)),
		InsertSettings(Move(string.InsertSettings)) {}

	PString::PString(const char8* text ...)
	{
		CL_PROFILE_FUNCTION_LOW()

			std::vector<Ambiguous> parameters;
		std::vector<int32> insertSymbolSize;
		std::vector<int32> insertSymbolIndex;
		std::vector<int32> insertReplace;

		va_list args;
		va_start(args, text);

		int32 insert = false;

		int32 insertIndexOffset = 0;

		int32 srcSize = 0;

		int32 settingIndex = -1;

		for (int32 i = 0; text[i] != '\0'; ++i)
		{
			if (text[i] == 127)
			{
				insertSymbolIndex.push_back(++i);
				insertSymbolSize.push_back(1);
				insertReplace.push_back(-1);
				++srcSize;
				while (text[i] == 127)
				{
					--insertIndexOffset;
					++insertSymbolSize.back();
					++i;
					++srcSize;
				}
			}

			++srcSize;

			if (!insert)
			{
				switch (text[i])
				{
				case '%':
					// %% are escaped to %
					if (text[i + 1] == '%')
					{
						insertSymbolIndex.push_back(++i);
						insertSymbolSize.push_back(1);
						insertReplace.push_back(-1);
						--insertIndexOffset;
						++srcSize;
						break;
					}

					insertSymbolIndex.push_back(i);
					insertSymbolSize.push_back(2);
					InsertIndex.push_back(i + insertIndexOffset);
					InsertSize.push_back(0);

					parameters.push_back({});
					insert = true;
					break;
				default:;
				}
			}
			else
			{
				if (settingIndex != -1 && text[i] != '}')
				{
					++insertSymbolSize.back();
					continue;
				}
				switch (text[i])
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					InsertSize.back() = InsertSize.back() * 10 + text[i] - '0';
					++insertSymbolSize.back();
					continue;

				case '{':
					++insertSymbolSize.back();
					settingIndex = i + 1;
					continue;
				case '}':
				{
					++insertSymbolSize.back();
					Settings settings = ParseSettings(text + settingIndex, i - settingIndex);

					InsertSettings.push_back(settings);
					settingIndex = -1;
					continue;
				}
				case 's':
					InsertType.push_back(text[i]);
					parameters.back().Char8Ptr = va_arg(args, char8*);
					break;
				case 'b':
				case 'd':
				case 'x':
					InsertType.push_back(text[i]);
					parameters.back().Int32 = va_arg(args, int32);
					break;
				case 'f':
					InsertType.push_back(text[i]);
					parameters.back().Float64 = va_arg(args, float64);
					break;
				case 'S':
					InsertType.push_back(text[i]);
					parameters.back().Char8Ptr = va_arg(args, char8*);
					InsertSize.back() += va_arg(args, int32);
					break;
				case 'B':
				case 'D':
				case 'X':
					InsertType.push_back(text[i]);
					parameters.back().Int32 = va_arg(args, int32);
					InsertSize.back() += va_arg(args, int32);
					break;
				case 'F':
					InsertType.push_back(text[i]);
					parameters.back().Float64 = va_arg(args, float64);
					InsertSize.back() += va_arg(args, int32);
					break;
				case 'R':
					InsertType.push_back(text[i]);
					parameters.back().VoidPtr = va_arg(args, void*);
					InsertSize.back() += static_cast<RString<>*>(parameters.back().VoidPtr)->Get()->GetLength();
					break;
				default:
					CL_ASSERT(false, "Unknow PString parameter type!");
				}

				// If current insert size is 0, assigns default size
				if (InsertSize.back() == 0)
					InsertSize.back() = TypeToSize(InsertType.back());

				if (InsertType.size() != InsertSettings.size())
					InsertSettings.emplace_back();

				insertReplace.push_back(static_cast<int32>(InsertIndex.size()) - 1);

				insertIndexOffset += InsertSize.back() - insertSymbolSize.back();
				insert = false;
				settingIndex = -1;
			}
		}

		va_end(args);

		/*---------------------Size computation---------------------*/
		Size = CStringLength(text);

		for (int32 size : insertSymbolSize)
			Size -= size;

		for (int32 size : InsertSize)
			Size += size;
		/*----------------------------------------------------------*/

		delete[] Data;
		Data = new char8[static_cast<size_t>(Size) + 1];

		// Clearing memory with zero length space character
		memset(Data, 127, static_cast<size_t>(Size));

		// Adding escape character
		Data[Size] = '\0';


		auto insertSymbolIndexItr = insertSymbolIndex.begin();
		auto insertSymbolSizeItr = insertSymbolSize.begin();
		auto insertReplaceItr = insertReplace.begin();

		int32 dataOffset = 0;
		int32 sourceOffset = 0;

		for (uint32 i = 0; i < insertSymbolIndex.size(); ++i)
		{
			const int copySize = *insertSymbolIndexItr - sourceOffset;
			memcpy(Data + dataOffset, text + sourceOffset, static_cast<size_t>(copySize));
			sourceOffset = *insertSymbolIndexItr++ + *insertSymbolSizeItr++;

			dataOffset += copySize;

			if (*insertReplaceItr != -1)
				dataOffset += InsertSize[*insertReplaceItr];

			++insertReplaceItr;
		}

		memcpy(Data + dataOffset, text + sourceOffset, static_cast<size_t>(srcSize) - sourceOffset);


		for (uint32 i = 0; i < InsertType.size(); ++i)
		{
			switch (InsertType[i])
			{
			case 's':
			case 'S':
				if (parameters[i].Int64 == 0)
					continue;
				SetIndex(i, parameters[i].Char8Ptr);
				break;
			case 'R':
				if (parameters[i].Int64 == 0)
					continue;
				SetIndex(i, *static_cast<RString<>*>(parameters[i].VoidPtr));
				break;
			case 'b':
			case 'B':
			case 'd':
			case 'D':
			case 'x':
			case 'X':
				SetIndex(i, parameters[i].Int32);
				break;
			case 'f':
			case 'F':
				SetIndex(i, parameters[i].Float64);
				break;
			default:
				CL_ASSERT(false, "Unknow PString parameter type!");
			}
		}

		RecalculateLength();
	}


	PString& PString::operator=(const PString& string)
	{
		return *this = PString(string);
	}

	PString& PString::operator=(PString&& string) noexcept
	{
		swap(*this, string);
		return *this;
	}


	void swap(PString& left, PString& right) noexcept
	{
		using std::swap;

		swap(static_cast<String&>(left), static_cast<String&>(right));
		swap(left.InsertIndex, right.InsertIndex);
		swap(left.InsertType, right.InsertType);
		swap(left.InsertSize, right.InsertSize);
		swap(left.InsertSettings, right.InsertSettings);
	}


	String PString::ToString() const
	{
		CL_PROFILE_FUNCTION_LOW()

			const int32 length = GetLength();
		auto* newData = new char8[static_cast<int64>(Length) + 1];

		char8* iteratorSrc = Data;
		char8* iteratorDst = newData;
		do
		{
			if (*iteratorSrc != 127)
			{
				*iteratorDst = *iteratorSrc;
				++iteratorDst;
			}
		} while (*iteratorSrc++);
		return String(&newData, length);
	}

	int32 PString::TypeToSize(char8 type)
	{
		switch (type)
		{
		case 's':
		case 'S':
			return 16;
		case 'R':
			return -1;
		case 'b':
		case 'B':
			return 34;
		case 'd':
		case 'D':
			return 12;
		case 'x':
		case 'X':
			return 10;
		case 'f':
		case 'F':
			return 16;
		default:
			CL_ASSERT(false, "Unknow PString parameter type!");
		}

		return 0;
	}


	void PString::SetIndex(int32 parameterIndex, const char8* text)
	{
		CL_PROFILE_FUNCTION_LOW()

			const int32 size = CStringLength(text);

		char8* index = Data + InsertIndex[parameterIndex];
		memset(index, 127, InsertSize[parameterIndex]);
		memcpy(index, text, size);

		RecalculateLength();
	}

	void PString::SetIndex(int32 parameterIndex, const RString<>& string)
	{
		CL_PROFILE_FUNCTION_LOW()

			char8* index = Data + InsertIndex[parameterIndex];
		memset(index, 127, InsertSize[parameterIndex]);
		memcpy(index, string->CString(), string->GetLength());

		RecalculateLength();
	}

	void PString::SetIndex(int32 parameterIndex, int64 value, int32 base)
	{
		CL_PROFILE_FUNCTION_LOW()

			char8* iterator = Data + InsertIndex[parameterIndex];
		memset(iterator, 127, InsertSize[parameterIndex]);

		CL_ASSERT(InsertSize[parameterIndex] < 1000, "out of bounds");

		iterator += static_cast<int64>(InsertSize[parameterIndex]) - 1;

		// Using ~ bitwise operator to get rid of negative sign
		uint64 operationalValue = value > 0 ? value : 1 + static_cast<uint64>(~value);

		int32 decimalPoint = InsertSettings[parameterIndex].DecimalPoint + 1;

		do
		{
			if (--decimalPoint == 0)
				*iterator-- = '.';

			const auto charValue = static_cast<char8>(operationalValue % base);

			*iterator-- = ('0' + charValue) + (charValue >= 10 ? 'A' - '9' - 1 : 0);
		} while (operationalValue /= base);



		while (--decimalPoint > 0)
			*iterator-- = '0';

		if (decimalPoint == 0)
		{
			*iterator-- = '.';
			*iterator-- = '0';
		}

		if (value < 0)
		{
			*iterator = '-';
		}

		RecalculateLength();
	}

	void PString::SetIndex(int32 parameterIndex, int64 value)
	{
		switch (InsertType[parameterIndex])
		{
		case 'b':
		case 'B':
			SetIndex(parameterIndex, value, 2);
			break;
		case 'd':
		case 'D':
			SetIndex(parameterIndex, value, 10);
			break;
		case 'x':
		case 'X':
			SetIndex(parameterIndex, value, 16);
			break;
		default:
			CL_ASSERT(false, "Unknow PString parameter type!");
		}
	}

	void PString::SetIndex(int32 parameterIndex, float64 value)
	{
		CL_PROFILE_FUNCTION_LOW()

			char8* index = Data + InsertIndex[parameterIndex];
		memset(index, 127, static_cast<size_t>(InsertSize[parameterIndex]));

		int32 decimalPoint = InsertSettings[parameterIndex].DecimalPoint;
		if (decimalPoint < 0)
			decimalPoint = 0;

		index += d2fixed_buffered_n(value, decimalPoint, index);

		if (InsertSettings[parameterIndex].DecimalPoint == 0)
			*index = '.';

		RecalculateLength();
	}


	void PString::RecalculateLength()
	{
		CL_PROFILE_FUNCTION_LOW()

			Length = Size;
		char8* iterator = Data;
		do
		{
			if (*iterator == 127)
				--Length;
		} while (*iterator++);
	}

	void PString::ResizeParameter(int32 index, int32 size)
	{
		const int64 resize = size - InsertSize[index];


		if (resize > 0)
		{
			Data = static_cast<char8*>(realloc(Data, Size + resize));

			const auto parameterEndpoint = Data + InsertIndex[index] + InsertSize[index];

			memmove(Data + InsertIndex[index] + size, parameterEndpoint, 1 + Size - InsertIndex[index] - InsertSize[index]);

			memset(parameterEndpoint, 127, resize);
		}
		else if (resize < 0)
		{
			const auto parameterEndpoint = Data + InsertIndex[index] + InsertSize[index];

			memmove(Data + InsertIndex[index] + size, parameterEndpoint, 1 + Size - InsertIndex[index] - InsertSize[index]);

			Data = static_cast<char8*>(realloc(Data, Size + resize));
		}
		Size += resize;

		InsertSize[index] = size;

		for (int32 i = index + 1; i < InsertIndex.size(); ++i)
			InsertIndex[i] += resize;

		RecalculateLength();
	}

	PString::Settings PString::ParseSettings(const char8* source, int32 size)
	{
		CL_PROFILE_FUNCTION_LOW()

			Settings settings;

		enum class ParseState
		{
			None,
			End,
			Key,
			DecimalPoint
		};

		ParseState state = ParseState::None;
		ParseState newState;

		char8* stateBegging = nullptr;
		char8* newStateBegging = nullptr;

		char8* iterator = const_cast<char8*>(source) - 1;
		char8* end = const_cast<char8*>(source) + size + 1;

		while (++iterator != end)
		{
			switch (*iterator)
			{
			case '#':
				newState = ParseState::Key;
				newStateBegging = iterator + 1;
				break;
			case '.':
				newState = ParseState::DecimalPoint;
				newStateBegging = iterator + 1;
				break;
			case '}':
				newState = ParseState::End;
				break;
			default:
				newState = ParseState::None;
				break;
			}

			if (newState != ParseState::None)
			{
				switch (state)
				{
				case ParseState::None:
					break;
				case ParseState::Key:
				{
					const int32 keySize = iterator - stateBegging;
					auto* key = new char8[static_cast<uint64>(keySize) + 1]{};
					memcpy(key, stateBegging, keySize);
					settings.Key = String(&key, keySize);
					break;
				}
				case ParseState::DecimalPoint:
					settings.DecimalPoint = ParseInt(stateBegging, static_cast<int32>(iterator - stateBegging));
					break;
				}

				state = newState;
				stateBegging = newStateBegging;
			}
		}

		return settings;
	}
}

#pragma once
#include <cstdint>

typedef uint8_t			byte;

typedef char			char8;
typedef unsigned char	uchar8;

typedef int8_t			int8;
typedef uint8_t			uint8;
typedef int16_t			int16;
typedef uint16_t		uint16;
typedef int32_t			int32;
typedef uint32_t		uint32;
typedef int64_t			int64;
typedef uint64_t		uint64;

typedef float			float32;
typedef double			float64;
typedef long double		float128;

union Ambiguous
{
	byte		Byte;

	char8		Char8;
	uchar8		Uchar8;

	int8		Int8;
	uint8		Uint8;
	int16		Int16;
	uint16		Uint16;
	int32		Int32;
	uint32		Uint32;
	int64		Int64;
	uint64		Uint64;

	float32		Float32;
	float64		Float64;
	float128	Float128;


	void*		VoidPtr;

	byte*		BytePtr;

	char8*		Char8Ptr;
	uchar8*		Uchar8Ptr;

	int8*		Int8Ptr;
	uint8*		Uint8Ptr;
	int16*		Int16Ptr;
	uint16*		Uint16Ptr;
	int32*		Int32Ptr;
	uint32*		Uint32Ptr;
	int64*		Int64Ptr;
	uint64*		Uint64Ptr;

	float32*	Float32Ptr;
	float64*	Float64Ptr;
	float128*	Float128Ptr;
};
#include "pch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>


namespace Coil
{
	OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height)
		: Width(width),
		  Height(height),
		  InternalColorFormat(GL_RGBA8),
		  ColorFormat(GL_RGBA)
	{
		CL_PROFILE_FUNCTION_HIGH()

		glCreateTextures(GL_TEXTURE_2D, 1, &TextureName);
		glTextureStorage2D(TextureName, 1, InternalColorFormat, Width, Height);

		glTextureParameteri(TextureName, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(TextureName, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(TextureName, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(TextureName, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const RString<>& path)
		: Path(path),
		  InternalColorFormat(0),
		  ColorFormat(0)
	{
		CL_PROFILE_FUNCTION_HIGH()

		int width, height, channels;
		stbi_uc* data;

		{
			CL_PROFILE_SCOPE_MEDIUM("Loading Texture")

			stbi_set_flip_vertically_on_load(1);
			data = stbi_load(path->CString(), &width, &height, &channels, 0);
			CL_CORE_ASSERT(data, "Failed to load image!");
		}

		Width  = width;
		Height = height;

		if (channels == 3)
		{
			InternalColorFormat = GL_RGB8;
			ColorFormat         = GL_RGB;
		}
		else if (channels == 4)
		{
			InternalColorFormat = GL_RGBA8;
			ColorFormat         = GL_RGBA;
		}

		CL_CORE_ASSERT(ColorFormat && InternalColorFormat, "Color format not supported!");

		{
			CL_PROFILE_SCOPE_MEDIUM("Creating Texture")

			glCreateTextures(GL_TEXTURE_2D, 1, &TextureName);
			glTextureStorage2D(TextureName, 1, InternalColorFormat, Width, Height);

			glTextureParameteri(TextureName, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(TextureName, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(TextureName, 0, 0, 0, Width, Height, ColorFormat, GL_UNSIGNED_BYTE, data);
		}

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		CL_PROFILE_FUNCTION_HIGH()

		glDeleteTextures(1, &TextureName);
	}

	void OpenGLTexture2D::SetData(void* data, uint32 size)
	{
		CL_PROFILE_FUNCTION_HIGH()

		uint32_t bpp = ColorFormat == GL_RGBA ? 4 : 3;
		CL_CORE_ASSERT(size == Width * Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(TextureName, 0, 0, 0, Width, Height, ColorFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		CL_PROFILE_FUNCTION_LOW()

		glBindTextureUnit(slot, TextureName);
	}
}

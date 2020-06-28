#include "pch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>


namespace Coil
{
	OpenGLTexture2D::OpenGLTexture2D(const RString<>& path)
		: Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path->CString(), &width, &height, &channels, 0);
		CL_CORE_ASSERT(data, "Failed to load image!");
		Width  = width;
		Height = height;


		GLenum internalColorFormat = 0;
		GLenum colorFormat         = 0;

		if (channels == 3)
		{
			internalColorFormat = GL_RGB8;
			colorFormat         = GL_RGB;
		}
		else if (channels == 4)
		{
			internalColorFormat = GL_RGBA8;
			colorFormat         = GL_RGBA;
		}

		CL_CORE_ASSERT(colorFormat && internalColorFormat, "Color format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &TextureName);
		glTextureStorage2D(TextureName, 1, internalColorFormat, Width, Height);

		glTextureParameteri(TextureName, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(TextureName, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(TextureName, 0, 0, 0, Width, Height, colorFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &TextureName);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, TextureName);
	}
}

#include "pch.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "stb_image.h"


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

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
		glTextureStorage2D(RendererID, 1, GL_RGB8, Width, Height);

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum colorFormat;

		switch (channels)
		{
		case 3:
			colorFormat = GL_RGB;
			break;
		case 4:
			colorFormat = GL_RGBA;
			break;
		default:
			colorFormat = GL_RGBA;
		}

		glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, colorFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, RendererID);
	}
}

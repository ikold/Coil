#pragma once

#include "Coil/Renderer/Texture.h"
#include <glad/glad.h>


namespace Coil
{
	/**
	 * @brief 
	 *
	 *
	 * @return		
	 *
	 * @todo Write a documentation
	 */
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32 width, uint32 height);
		explicit OpenGLTexture2D(const RString<>& path);

		~OpenGLTexture2D() override;

		bool operator==(const Texture& other) const override;

		[[nodiscard]] uint32 GetWidth() const override { return Width; }
		[[nodiscard]] uint32 GetHeight() const override { return Height; }

		void SetData(void* data, uint32 size) override;

		void Bind(uint32 slot = 0) const override;

	private:
		RString<> Path;
		uint32 Width, Height;
		uint32 TextureName{};
		GLenum InternalColorFormat, ColorFormat;
	};
}

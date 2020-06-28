#pragma once

#include "Coil/Renderer/Texture.h"


namespace Coil
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		explicit OpenGLTexture2D(const RString<>& path);
		~OpenGLTexture2D() override;

		uint32 GetWidth() const override { return Width; }
		uint32 GetHeight() const override { return Height; }

		void Bind(uint32 slot = 0) const override;

	private:
		RString<> Path;
		uint32 Width, Height;
		uint32 RendererID;
	};
}

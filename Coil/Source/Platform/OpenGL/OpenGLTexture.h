#pragma once

#include "Coil/Renderer/Texture.h"


namespace Coil
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const RString<>& path);
		virtual ~OpenGLTexture2D();

		uint32 GetWidth() const override { return Width; }
		uint32 GetHeight() const override { return Height; }

		void Bind(uint32 slot = 0) const override;

	private:
		RString<String> Path;
		uint32 Width, Height;
		uint32 RendererID;
	};
}

#pragma once


namespace Coil
{
	class Shader
	{
	public:
		Shader(RString<String> vertexSource, RString<String> fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32 RendererID;
	};
}
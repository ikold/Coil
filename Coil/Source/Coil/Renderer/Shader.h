#pragma once

#include <glm/glm.hpp>

namespace Coil
{
	class Shader
	{
	public:
		Shader(RString<String> vertexSource, RString<String> fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const RString<String> name, const glm::mat4& matrix) const;
	private:
		uint32 RendererID;
	};
}

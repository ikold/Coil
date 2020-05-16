#pragma once

#include "Coil/Renderer/Shader.h"
#include <glm/glm.hpp>


namespace Coil
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const RString<String>& vertexSource, const RString<String>& fragmentSource);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;


		void UploadUniformInt(const RString<String>& name, int32 value) const;
		void UploadUniformFloat(const RString<String>& name, float32 value) const;

		void UploadUniformFloat2(const RString<String>& name, const glm::vec2& vector) const;
		void UploadUniformFloat3(const RString<String>& name, const glm::vec3& vector) const;
		void UploadUniformFloat4(const RString<String>& name, const glm::vec4& vector) const;

		void UploadUniformMat3(const RString<String>& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const RString<String>& name, const glm::mat4& matrix) const;

	private:
		uint32 RendererID;
	};
}

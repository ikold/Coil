#pragma once

#include "Coil/Renderer/Shader.h"
#include <glm/glm.hpp>


namespace Coil
{
	class OpenGLShader final : public Shader
	{
	public:
		OpenGLShader(const RString<>& name, const RString<>& vertexSource, const RString<>& fragmentSource);
		explicit OpenGLShader(const RString<>& filePath);

		~OpenGLShader() override;

		static std::unordered_map<uint32, RString<>> PreProcess(const RString<>& source);

		void Compile(const std::unordered_map<uint32, RString<>>& shaderSources);

		void Bind() const override;
		void Unbind() const override;


		const RString<>& GetName() const override { return Name; };


		void UploadUniformInt(const RString<>& name, int32 value) const;
		void UploadUniformFloat(const RString<>& name, float32 value) const;

		void UploadUniformFloat2(const RString<>& name, const glm::vec2& vector) const;
		void UploadUniformFloat3(const RString<>& name, const glm::vec3& vector) const;
		void UploadUniformFloat4(const RString<>& name, const glm::vec4& vector) const;

		void UploadUniformMat3(const RString<>& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const RString<>& name, const glm::mat4& matrix) const;

	private:
		uint32 RendererID;
		RString<> Name;
	};
}

#pragma once

#include "Coil/Renderer/Shader.h"


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
	class OpenGLShader final : public Shader
	{
	public:
		explicit OpenGLShader(const RString<>& filePath);
		OpenGLShader(RString<> name, const RString<>& vertexSource, const RString<>& fragmentSource);

		~OpenGLShader() override;

		static std::unordered_map<uint32, RString<>> PreProcess(const RString<>& source);

		void Compile(const std::unordered_map<uint32, RString<>>& shaderSources);

		void Bind() const override;
		void Unbind() const override;


		[[nodiscard]] const RString<>& GetName() const override;


		void SetInt(const RString<>& name, int32 value) const override;
		void SetIntArray(const RString<>& name, int32* data, uint32 count) const override;
		void SetFloat(const RString<>& name, float32 value) const override;
		
		void SetFloat2(const RString<>& name, const glm::vec2& vector) const override;
		void SetFloat3(const RString<>& name, const glm::vec3& vector) const override;
		void SetFloat4(const RString<>& name, const glm::vec4& vector) const override;

		void SetMat3(const RString<>& name, const glm::mat3& matrix) const override;
		void SetMat4(const RString<>& name, const glm::mat4& matrix) const override;


		void UploadUniformInt(const RString<>& name, int32 value) const;
		void UploadUniformIntArray(const RString<>& name, int32* data, uint32 count) const;
		void UploadUniformFloat(const RString<>& name, float32 value) const;

		void UploadUniformFloat2(const RString<>& name, const glm::vec2& vector) const;
		void UploadUniformFloat3(const RString<>& name, const glm::vec3& vector) const;
		void UploadUniformFloat4(const RString<>& name, const glm::vec4& vector) const;

		void UploadUniformMat3(const RString<>& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const RString<>& name, const glm::mat4& matrix) const;

	private:
		uint32 RendererID{};
		RString<> Name;
	};
}

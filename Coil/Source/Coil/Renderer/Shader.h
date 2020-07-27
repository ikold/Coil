#pragma once


namespace Coil
{
	class Shader
	{
	public:
		Shader()                  = default;
		Shader(const Shader& rhs) = default;
		Shader(Shader&& rhs)      = default;

		virtual ~Shader() = default;

		Shader& operator=(const Shader& rhs) = default;
		Shader& operator=(Shader&& rhs)      = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


		virtual void SetInt(const RString<>& name, int32 value) const = 0;
		virtual void SetFloat(const RString<>& name, float32 value) const = 0;

		virtual void SetFloat2(const RString<>& name, const glm::vec2& vector) const = 0;
		virtual void SetFloat3(const RString<>& name, const glm::vec3& vector) const = 0;
		virtual void SetFloat4(const RString<>& name, const glm::vec4& vector) const = 0;

		virtual void SetMat3(const RString<>& name, const glm::mat3& matrix) const = 0;
		virtual void SetMat4(const RString<>& name, const glm::mat4& matrix) const = 0;


		[[nodiscard]] virtual const RString<>& GetName() const = 0;

		static Ref<Shader> Create(const RString<>& filePath);

		static Ref<Shader> Create(const RString<>& name, const RString<>& vertexSource, const RString<>& fragmentSource);
	};


	class ShaderLibrary
	{
	public:
		void Add(const RString<>& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const RString<>& filePath);
		Ref<Shader> Load(const RString<>& name, const RString<>& filePath);

		Ref<Shader> Get(const RString<>& name);

		[[nodiscard]] bool Exists(const RString<>& name) const;

	private:
		std::unordered_map<RString<>, Ref<Shader>> Shaders;
	};
}

#pragma once


namespace Coil
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual const RString<>& GetName() const = 0;

		static Ref<Shader> Create(const RString<>& filePath);

		static Ref<Shader> Create(const RString<>& name, const RString<>& vertexSource, const RString<>& fragmentSource);
	};


	class ShaderLibrary
	{
	public:
		void Add(const RString<>& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const RString<>& filepath);
		Ref<Shader> Load(const RString<>& name, const RString<>& filepath);

		Ref<Shader> Get(const RString<>& name);

		[[nodiscard]] bool Exists(const RString<>& name) const;

	private:
		std::unordered_map<RString<>, Ref<Shader>> Shaders;
	};
};

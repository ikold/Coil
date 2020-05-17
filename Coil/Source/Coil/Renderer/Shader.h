#pragma once


namespace Coil
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<Shader> Create(const RString<String>& vertexSource, const RString<String>& fragmentSource);
	};
}

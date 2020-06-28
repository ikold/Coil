#include "pch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Coil
{
	Ref<Shader> Shader::Create(const RString<>& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		CL_CORE_ASSERT(false, "RenderAPI::None unsupported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filePath);
		}

		CL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const RString<>& name, const RString<>& vertexSource, const RString<>& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		CL_CORE_ASSERT(false, "RenderAPI::None unsupported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		CL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const RString<>& name, const Ref<Shader>& shader)
	{
		CL_CORE_ASSERT(!Exists(name), "Shader already exists!");
		Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const RString<>& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const RString<>& name, const RString<>& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const RString<>& name)
	{
		CL_CORE_ASSERT(Exists(name), "Shader not found!");
		return Shaders[name];
	}

	bool ShaderLibrary::Exists(const RString<>& name) const
	{
		return Shaders.find(name) != Shaders.end();
	}
}

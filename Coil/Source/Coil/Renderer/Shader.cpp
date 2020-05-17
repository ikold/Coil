#include "pch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Coil
{
	Ref<Shader> Shader::Create(const RString<String>& vertexSource, const RString<String>& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		CL_CORE_ASSERT(false, "RenderAPI::None unsupported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		}

		CL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

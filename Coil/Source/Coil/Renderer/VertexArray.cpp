#include "pch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Coil
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CL_CORE_ASSERT(false, "RenderAPI::None unsuported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		CL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
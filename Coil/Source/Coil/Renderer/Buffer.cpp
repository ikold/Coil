#include "pch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"


namespace Coil
{
	VertexBuffer* VertexBuffer::Create(float32* vertices, uint32 size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CL_CORE_ASSERT(false, "RenderAPI::None unsuported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		CL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32* indices, uint32 size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: CL_CORE_ASSERT(false, "RenderAPI::None unsuported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		CL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>


namespace Coil
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32 size)
	{
		CL_PROFILE_FUNCTION_HIGH()

		glCreateBuffers(1, &RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float32* vertices, uint32 size)
	{
		CL_PROFILE_FUNCTION_HIGH()

		glCreateBuffers(1, &RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		CL_PROFILE_FUNCTION_HIGH()

		glDeleteBuffers(1, &RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		CL_PROFILE_FUNCTION_HIGH()

		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		CL_PROFILE_FUNCTION_HIGH()

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32 size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count)
		: Count(count)
	{
		CL_PROFILE_FUNCTION_HIGH()

		glCreateBuffers(1, &RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		CL_PROFILE_FUNCTION_HIGH()

		glDeleteBuffers(1, &RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		CL_PROFILE_FUNCTION_HIGH()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		CL_PROFILE_FUNCTION_HIGH()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

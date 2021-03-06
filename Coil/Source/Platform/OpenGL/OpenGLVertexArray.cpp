#include "pch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>


namespace Coil
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4: return GL_FLOAT;

		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4: return GL_INT;

		case ShaderDataType::Bool: return GL_BOOL;
		default:
		CL_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		CL_PROFILE_FUNCTION_HIGH()

		glGenVertexArrays(1, &RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		CL_PROFILE_FUNCTION_HIGH()

		glDeleteVertexArrays(1, &RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		CL_PROFILE_FUNCTION_LOW()

		glBindVertexArray(RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		CL_PROFILE_FUNCTION_LOW()

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		CL_PROFILE_FUNCTION_HIGH()

		CL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(VertexBufferIndex);
			glVertexAttribPointer(VertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				reinterpret_cast<const void*>(static_cast<int64>(element.Offset)));
			++VertexBufferIndex;
		}

		mVertexBuffer.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		CL_PROFILE_FUNCTION_HIGH()

		glBindVertexArray(RendererID);
		indexBuffer->Bind();

		mIndexBuffer = indexBuffer;
	}
}

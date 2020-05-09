#pragma once

#include "Coil/Core.h"
#include "Coil/Renderer/Buffer.h"

namespace Coil
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float32* vertices, uint32 size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return Layout; }

	private:
		uint32 RendererID;
		BufferLayout Layout;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32* indicies, uint32 count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32 GetCount() const override { return Count; };

	private:
		uint32 RendererID;
		uint32 Count;
	};
}

#pragma once

#include "Coil/Renderer/Buffer.h"

namespace Coil
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float32* vertices, uint32 size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetLayout(const BufferLayout& layout) override { Layout = layout; }
		[[nodiscard]] const BufferLayout& GetLayout() const override { return Layout; }

	private:
		uint32 RendererID{};
		BufferLayout Layout;
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32* indices, uint32 count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		[[nodiscard]] uint32 GetCount() const override { return Count; }

	private:
		uint32 RendererID{};
		uint32 Count;
	};
}

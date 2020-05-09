#pragma once

#include "Coil/Core.h"

#include "Coil/Renderer/VertexArray.h"

namespace Coil
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return mVertexBuffer; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return mIndexBuffer; }

	private:
		uint32 RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
	};
}
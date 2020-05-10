#pragma once

#include "Coil/Renderer/VertexArray.h"


namespace Coil
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		[[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return mVertexBuffer; }
		[[nodiscard]] const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return mIndexBuffer; }

	private:
		uint32 RendererID{};
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
	};
}

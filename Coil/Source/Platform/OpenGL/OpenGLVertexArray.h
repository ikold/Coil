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

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		[[nodiscard]] const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return mVertexBuffer; }
		[[nodiscard]] const Ref<IndexBuffer>& GetIndexBuffer() const override { return mIndexBuffer; }

	private:
		uint32 RendererID{};
		std::vector<Ref<VertexBuffer>> mVertexBuffer;
		Ref<IndexBuffer> mIndexBuffer;
	};
}

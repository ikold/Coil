#pragma once

#include "Coil/Renderer/Buffer.h"


namespace Coil
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& vertexBuffer) = 0;

		[[nodiscard]] virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		[[nodiscard]] virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}

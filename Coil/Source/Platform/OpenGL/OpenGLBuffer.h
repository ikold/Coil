#pragma once

#include "Coil/Renderer/Buffer.h"


namespace Coil
{
	/**
	 * @brief 
	 *
	 *
	 * @return		
	 *
	 * @todo Write a documentation
	 */
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:
		explicit OpenGLVertexBuffer(uint32 size);
		OpenGLVertexBuffer(float32* vertices, uint32 size);

		~OpenGLVertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32 size) override;

		void SetLayout(const BufferLayout& layout) override { Layout = layout; }
		[[nodiscard]] const BufferLayout& GetLayout() const override { return Layout; }

	private:
		uint32 RendererID{};
		BufferLayout Layout;
	};


	/**
	 * @brief 
	 *
	 *
	 * @return		
	 *
	 * @todo Write a documentation
	 */
	class OpenGLIndexBuffer final : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32* indices, uint32 count);

		~OpenGLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		[[nodiscard]] uint32 GetCount() const override { return Count; }

	private:
		uint32 RendererID{};
		uint32 Count;
	};
}

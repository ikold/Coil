#pragma once

#include "RendererAPI.h"


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
	class RenderCommand
	{
	public:
		static void Init() { sRendererAPI->Init(); }

		static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) { sRendererAPI->SetViewport(x, y, width, height); }

		static void SetClearColor(const glm::vec4& color) { sRendererAPI->SetClearColor(color); }
		static void Clear() { sRendererAPI->Clear(); }

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 indexCount = 0) { sRendererAPI->DrawIndex(vertexArray, indexCount); }

	private:
		static Scope<RendererAPI> sRendererAPI;
	};
}

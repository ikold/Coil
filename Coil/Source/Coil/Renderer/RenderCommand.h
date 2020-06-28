#pragma once

#include "RendererAPI.h"


namespace Coil
{
	class RenderCommand
	{
	public:
		static void Init() { sRendererAPI->Init(); }

		static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) { sRendererAPI->SetViewport(x, y, width, height); }

		static void SetClearColor(const glm::vec4& color) { sRendererAPI->SetClearColor(color); }
		static void Clear() { sRendererAPI->Clear(); }

		static void DrawIndexed(const Ref<VertexArray>& vertexArray) { sRendererAPI->DrawIndex(vertexArray); }

	private:
		static Scope<RendererAPI> sRendererAPI;
	};
}

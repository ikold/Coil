#pragma once

#include "RendererAPI.h"


namespace Coil
{
	class RenderCommand
	{
	public:
		static void SetClearColor(const glm::vec4& color) { sRendererAPI->SetClearColor(color); }
		static void Clear() { sRendererAPI->Clear(); }

		static void DrawIndexed(const Ref<VertexArray>& vertexArray) { sRendererAPI->DrawIndex(vertexArray); }
	private:
		static RendererAPI* sRendererAPI;
	};
}

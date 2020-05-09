#pragma once

#include "Coil/Core.h"

#include "RendererAPI.h"

namespace Coil
{
	class RenderCommand
	{
	public:
		static void SetClearColor(const glm::vec4& color) { sRenererAPI->SetClearColor(color); }
		static void Clear() { sRenererAPI->Clear(); }

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)	{ sRenererAPI->DrawIndex(vertexArray); }
	private:
		static RendererAPI* sRenererAPI;
	};

}


#pragma once

#include "Coil/Core.h"

#include "RendererAPI.h"

namespace Coil
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color) { sRenererAPI->SetClearColor(color); }
		inline static void Clear() { sRenererAPI->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)	{ sRenererAPI->DrawIndex(vertexArray); }
	private:
		static RendererAPI* sRenererAPI;
	};

}


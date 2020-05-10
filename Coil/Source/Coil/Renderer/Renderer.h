#pragma once

#include "RenderCommand.h"


namespace Coil
{
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

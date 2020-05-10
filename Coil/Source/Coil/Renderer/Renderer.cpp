#include "pch.h"
#include "Renderer.h"


namespace Coil
{
	void Renderer::BeginScene()
	{}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		RenderCommand::DrawIndexed(vertexArray);
	}
}

#include "pch.h"
#include "Renderer.h"

#include "Renderer2D.h"


namespace Coil
{
	Scope<Renderer::SceneData> Renderer::sSceneData = CreateScope<SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		sSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		static RString<> uViewProjection = "uViewProjection";
		static RString<> uTransform      = "uTransform";

		shader->SetMat4(uViewProjection, sSceneData->ViewProjectionMatrix);
		shader->SetMat4(uTransform, transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32 width, uint32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}

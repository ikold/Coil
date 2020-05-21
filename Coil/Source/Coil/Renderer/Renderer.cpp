#include "pch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Coil
{
	Renderer::SceneData* Renderer::sSceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		sSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		static RString<String> uViewProjection = "uViewProjection";
		static RString<String> uTransform = "uTransform";
		
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(uViewProjection, sSceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(uTransform, transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}

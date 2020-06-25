#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"


namespace Coil
{
	class Renderer
	{
	public:
		static void Init();
		
		static void OnWindowResize(uint32 width, uint32 height);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};


		static SceneData* sSceneData;
	};
}

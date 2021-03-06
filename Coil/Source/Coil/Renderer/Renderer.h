#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"


namespace Coil
{
	/**
	 * @brief 3D renderer
	 *
	 * @todo Write a documentation
	 */
	class Renderer
	{
	public:
		static void Init();

		static void ShutDown();

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


		static Scope<SceneData> sSceneData;
	};
}

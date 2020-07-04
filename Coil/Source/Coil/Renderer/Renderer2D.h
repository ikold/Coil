#pragma once

#include <memory>

#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"


namespace Coil
{
	class Renderer2D
	{
	public:
		static void Init();
		static void ShoutDown();
		
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, float32 rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const glm::vec4& color);
		
		static void DrawQuad(const glm::vec2& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Shader>& shader);
	};
}

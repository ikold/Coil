#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"


namespace Coil
{
	/**
	 * @brief Quad base 2D renderer
	 *
	 * @todo Write a documentation
	 */
	class Renderer2D
	{
	public:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TextureCoordinates;
			float32 TextureIndex;
		};

		struct Statistics
		{
			uint32 DrawCalls = 0;
			uint32 QuadCount = 0;

			[[nodiscard]] uint32 GetTotalVertexCount() const { return QuadCount * 4; }
			[[nodiscard]] uint32 GetTotalIndexCount() const { return QuadCount * 6; }
		};

		class QuadBuilder
		{
		public:
			QuadBuilder();

			void SetPosition(const glm::vec3& position);
			void SetPosition(const glm::vec2& position);

			void SetZPosition(float32 zPosition);

			void SetScale(const glm::vec2& scale);

			void SetRotation(float32 radians);

			void SetTexture(const Ref<Texture2D>& texture);

			void ResetTexture();

			void SetColor(const glm::vec4& color);

			void Move(const glm::vec3& position);
			void Move(const glm::vec2& position);


			void Revalidate();

			void Draw() const;

		private:
			glm::vec3 Position{};
			glm::vec2 Scale{};
			float32 Rotation{};
			Ref<Texture2D> Texture{};
			glm::vec4 Color{};

			glm::vec2 TransformedQVP[4];

			QuadVertex QuadData[4]{};
		};

	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void ResetStatistics();
		static Statistics GetStatistics();

		static uint32 GetSizeOfQuad();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.f));
	};
}

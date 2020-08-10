#pragma once


namespace Coil
{
	/**
	 * @brief Orthographic camera for drawing 2D scenes
	 */
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float32 left, float32 right, float32 bottom, float32 top);

		[[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position);
		void SetRotation(float32 rotation);

		void SetProjection(float32 left, float32 right, float32 bottom, float32 top);

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 ProjectionMatrix{};
		glm::mat4 ViewMatrix{};
		glm::mat4 ViewProjectionMatrix{};

		glm::vec3 Position;
		float32 Rotation;
	};
}

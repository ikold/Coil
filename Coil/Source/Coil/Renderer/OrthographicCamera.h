#pragma once

#include <glm/glm.hpp>


namespace Coil
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float32 left, float32 right, float32 bottom, float32 top);

		[[nodiscard]] const glm::vec3& GetPosition() const { return Position; }
		[[nodiscard]] float32 GetRotation() const { return Rotation; }

		[[nodiscard]] const glm::mat4& GetViewProjectionMatrix();

		void SetPosition(const glm::vec3& position);
		void SetScale(const glm::vec3& scale);
		void SetRotation(float32 rotation);

		void MovePosition(const glm::vec3& position);
		void MoveRelativePosition(const glm::vec3& position);
		
	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix{};
		glm::mat4 ViewProjectionMatrix{};

		glm::vec3 Position;
		glm::vec3 Scale;
		float32 Rotation;

		bool ProjectionMatrixIsValid = false;
	};
}

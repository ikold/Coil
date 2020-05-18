#include "pch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Coil
{
	OrthographicCamera::OrthographicCamera(float32 left, float32 right, float32 bottom, float32 top)
		: ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.f, 1.f)),
		  Position(0.f, 0.f, 0.f),
		  Scale(1.f, 1.f, 1.f),
		  Rotation(0.f)
	{
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetScale(const glm::vec3& scale)
	{
		Scale = scale;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotation(float32 rotation)
	{
		Rotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		const glm::mat4 scale       = glm::scale(glm::mat4(1.f), glm::vec3(Scale));
		const glm::mat4 translation = glm::translate(glm::mat4(1.f), Position);
		const glm::mat4 rotation    = glm::rotate(glm::mat4(1.f), glm::radians(Rotation), glm::vec3(0.f, 0.f, 1.f));

		glm::mat4 transform = scale * translation * rotation;

		ViewMatrix           = glm::inverse(transform);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}
}

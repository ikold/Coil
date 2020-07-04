#include "pch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Coil
{
	OrthographicCamera::OrthographicCamera(float32 left, float32 right, float32 bottom, float32 top)
		: ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.f, 1.f)),
		  Position(0.f, 0.f, 0.f),
		  Rotation(0.f)
	{
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotation(float32 rotation)
	{
		Rotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float32 left, float32 right, float32 bottom, float32 top)
	{
		ProjectionMatrix     = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		const glm::mat4 transform = rotate(translate(glm::mat4(1.f), Position), glm::radians(Rotation), glm::vec3(0.f, 0.f, 1.f));

		ViewMatrix           = inverse(transform);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}
}

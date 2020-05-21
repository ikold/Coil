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

	const glm::mat4& OrthographicCamera::GetViewProjectionMatrix()
	{
		if (!ProjectionMatrixIsValid)
			RecalculateViewMatrix();

		return ViewProjectionMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		ProjectionMatrixIsValid = false;
		
		Position = position;
	}

	void OrthographicCamera::SetScale(const glm::vec3& scale)
	{
		ProjectionMatrixIsValid = false;
		
		Scale = scale;
	}

	void OrthographicCamera::SetRotation(float32 rotation)
	{
		ProjectionMatrixIsValid = false;

		Rotation = rotation;
	}

	void OrthographicCamera::MovePosition(const glm::vec3& position)
	{
		ProjectionMatrixIsValid = false;

		Position.x += position.x;
		Position.y += position.y;
		Position.z += position.z;
	}

	void OrthographicCamera::MoveRelativePosition(const glm::vec3& position)
	{
		ProjectionMatrixIsValid = false;

		const float32 rSin = glm::sin(glm::radians(Rotation));
		const float32 rCos = glm::cos(glm::radians(Rotation));

		Position.x += Scale.x * (rCos * position.x + rSin * position.y);

		Position.y += Scale.y * (rSin * position.x - rCos * position.y);

		Position.z += Scale.z * position.z;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		const glm::mat4 scale       = glm::scale(glm::mat4(1.f), glm::vec3(Scale));
		const glm::mat4 translation = glm::translate(glm::mat4(1.f), Position);
		const glm::mat4 rotation    = glm::rotate(glm::mat4(1.f), glm::radians(Rotation), glm::vec3(0.f, 0.f, 1.f));

		glm::mat4 transform = translation * rotation * scale;

		ViewMatrix           = glm::inverse(transform);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

		ProjectionMatrixIsValid = true;
	}
}

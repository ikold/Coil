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
		CL_PROFILE_FUNCTION_HIGH()

		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		Position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotation(float32 rotation)
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		Rotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float32 left, float32 right, float32 bottom, float32 top)
	{
		CL_PROFILE_FUNCTION_MEDIUM()

		ProjectionMatrix     = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		CL_PROFILE_FUNCTION_MEDIUM()

		const float32 c = cos(glm::radians(Rotation));
		const float32 s = sin(glm::radians(Rotation));

		const glm::mat4 transform = {
			{ c,			s,			0.f,		0.f },
			{ -s,			c,			0.f,		0.f },
			{ 0.f,			0.f,		1.f,		0.f },
			{ Position.x,	Position.y,	Position.z,	1.f },
		};

		ViewMatrix           = inverse(transform);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}
}

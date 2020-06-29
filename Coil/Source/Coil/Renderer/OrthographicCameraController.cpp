#include "pch.h"
#include "OrthographicCameraController.h"


namespace Coil
{
	OrthographicCameraController::OrthographicCameraController(float32 aspectRatio, bool rotation)
		: AspectRatio(aspectRatio),
		  Camera(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel),
		  Rotation(rotation)
	{}

	void OrthographicCameraController::OnUpdate()
	{
		if (Input::IsKeyPressed(CL_KEY_A))
		{
			CameraPosition.x -= cos(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
			CameraPosition.y -= sin(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
		}
		else if (Input::IsKeyPressed(CL_KEY_D))
		{
			CameraPosition.x += cos(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
			CameraPosition.y += sin(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
		}

		if (Input::IsKeyPressed(CL_KEY_W))
		{
			CameraPosition.x += -sin(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
			CameraPosition.y += cos(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
		}
		else if (Input::IsKeyPressed(CL_KEY_S))
		{
			CameraPosition.x -= -sin(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
			CameraPosition.y -= cos(glm::radians(CameraRotation)) * CameraTranslationSpeed * Time::DeltaTime() / 1000.f;
		}

		if (Rotation)
		{
			if (Input::IsKeyPressed(CL_KEY_Q))
				CameraRotation += CameraRotationSpeed * Time::DeltaTime() / 1000.f;
			if (Input::IsKeyPressed(CL_KEY_E))
				CameraRotation -= CameraRotationSpeed * Time::DeltaTime() / 1000.f;

			Camera.SetRotation(CameraRotation);
		}

		Camera.SetPosition(CameraPosition);

		CameraTranslationSpeed = ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_METHOD(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_METHOD(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		ZoomLevel -= e.GetYOffset() * 0.25f;
		ZoomLevel = std::max(ZoomLevel, 0.25f);
		Camera.SetProjection(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		AspectRatio = static_cast<float32>(e.GetWidth()) / static_cast<float32>(e.GetHeight());
		Camera.SetProjection(-AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel);
		return false;
	}
}

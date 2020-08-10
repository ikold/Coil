#pragma once

#include "Coil/Renderer/OrthographicCamera.h"


namespace Coil
{
	/**
	 * @brief Event base orthographic camera controller
	 */
	class OrthographicCameraController
	{
	public:
		explicit OrthographicCameraController(float32 aspectRatio, bool rotation = false);

		void OnUpdate();
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return Camera; }
		[[nodiscard]] const OrthographicCamera& GetCamera() const { return Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float AspectRatio;
		float ZoomLevel = 1.0f;
		OrthographicCamera Camera;

		bool Rotation;

		glm::vec3 CameraPosition       = { 0.0f, 0.0f, 0.0f };
		float32 CameraRotation         = 0.0f;
		float32 CameraTranslationSpeed = 1.0f, CameraRotationSpeed = 180.0f;
	};
}

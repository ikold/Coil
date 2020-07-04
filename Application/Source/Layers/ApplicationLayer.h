#pragma once
#include "Coil.h"


class ApplicationLayer final : public Coil::Layer
{
public:
	ApplicationLayer();

	void OnUpdate() override;

	void OnEvent(Coil::Event& event) override;

private:
	Coil::Ref<Coil::Texture2D> Texture;

	Coil::OrthographicCameraController CameraController;

	Coil::RString<Coil::PString> FrameTime, MousePosition;

	Coil::Ref<glm::vec4> SquareColor;

	float32 TimeIteration = 0.f;
};

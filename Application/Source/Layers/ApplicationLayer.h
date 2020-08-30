#pragma once
#include "Coil.h"


/**
 * @brief Demo layer for testing the Coil engine functionality
 */
class ApplicationLayer final : public Coil::Layer
{
public:
	ApplicationLayer();

	void OnUpdate() override;

	void OnEvent(Coil::Event& event) override;

	bool OnKeyPressed(Coil::KeyPressedEvent& event) const;
	bool OnKeyReleased(Coil::KeyReleasedEvent& event) const;

private:
	Coil::Ref<Coil::Texture2D> Texture;

	Coil::OrthographicCameraController CameraController;

	Coil::RString<Coil::PString> FrameTime, MousePosition;

	Coil::Ref<glm::vec4> SquareColor;

	float32 Timer{};
	Coil::RString<Coil::PString> TimerString;
	
	Coil::RString<Coil::PString> InstrumentorStatisticsString;

	float32 TimeIteration = 0.f;
};

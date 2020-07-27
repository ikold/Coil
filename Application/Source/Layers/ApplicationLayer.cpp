#include "ApplicationLayer.h"

#include <glm/ext/scalar_constants.hpp>


ApplicationLayer::ApplicationLayer()
	: Layer("Application"),
	  CameraController(1280.0f / 720.0f, true),
	FrameTime(Coil::PString("%{.3}8f ms", 0.f)),
	  MousePosition(Coil::PString("mouse x: %6d y: %6d", 0, 0)),
	  SquareColor(Coil::CreateRef<glm::vec4>(0.2f, 0.2f, 0.2, 1.f)),
	TimerString(Coil::PString("%{.2}8f seconds remains", 0.f))
{
	CL_PROFILE_FUNCTION_HIGH()

	//Coil::Application::Get().GetWindow().SetVSync(false);
	Coil::GUI::LogWindow({ "Log" })->BindBuffer(Coil::Logger::GetBuffer());

	Coil::GUI::Overlay({ "frame time" })->BindTextBuffer(FrameTime);

	Coil::GUI::ComponentWindow({ "Name" }, {
		Coil::GUI::Text("Square Color"),
		Coil::GUI::ColorPicker({ "", -1 }, SquareColor),
		Coil::GUI::Button({ "Profile (High)" }, [&]
		{
			Timer          = 60.f;
			auto timestamp = Coil::Time::NowString();
			timestamp->Replace(':', '_');
			CL_PROFILE_BEGIN_SESSION_HIGH("High Profiling", Coil::PString("Profiling/CoilProfileHigh1m-Runtime %R.json", &timestamp));
		}),
		Coil::GUI::Button({ "Profile (Low)" }, [&]
		{
			Timer          = 60.f;
			auto timestamp = Coil::Time::NowString();
			timestamp->Replace(':', '_');
			CL_PROFILE_BEGIN_SESSION_LOW("High Profiling", Coil::PString("Profiling/CoilProfileLow1m-Runtime %R.json", &timestamp));
		})
	});

	Coil::Logger::Trace(MousePosition);
	Coil::Logger::Trace(TimerString);

	Texture = Coil::Texture2D::Create("Resources/Textures/Colorgrid.png");
}

void ApplicationLayer::OnUpdate()
{
	CL_PROFILE_FUNCTION_HIGH()

	FrameTime->Set(0, Coil::Time::DeltaTime());

	CameraController.OnUpdate();

	if (Timer > 0.f)
	{
		Timer -= Coil::Time::DeltaTime() / 1000.f;

		if (Timer <= 0.f)
		{
			Timer = 0.f;
			CL_PROFILE_END_SESSION()
		}
	}

	TimerString->Set(0, Timer);

	auto [mouseX, mouseY] = Coil::Input::GetMousePosition();
	MousePosition->Set(0, static_cast<int64>(mouseX));
	MousePosition->Set(1, static_cast<int64>(mouseY));

	{
		CL_PROFILE_SCOPE_HIGH("Rendering")

		Coil::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Coil::RenderCommand::Clear();

		Coil::Renderer2D::BeginScene(CameraController.GetCamera());

		const int32 gridHeight = 20;
		const int32 gridWidth  = 40;

		TimeIteration += Coil::Time::DeltaTime() / 1000.f * 0.5f;

		while (TimeIteration > 4.f)
			TimeIteration -= 4.f;

		Coil::Renderer2D::DrawQuad(glm::vec3(0.f), TimeIteration / 2 * glm::pi<float32>(), glm::vec2(1.f), Texture);

		{
			CL_PROFILE_SCOPE_HIGH("Grid rendering")
			for (int32 x = -gridWidth / 2; x < gridWidth - gridWidth / 2; ++x)
			{
				for (int32 y = -gridHeight / 2; y < gridHeight - gridHeight / 2; ++y)
					Coil::Renderer2D::DrawQuad({ (x - TimeIteration) * 0.11f, (y + TimeIteration / 4) * 0.11f }, 0.f, { 0.1f, 0.1f }, *SquareColor);
			}
		}

		Coil::Renderer2D::EndScene();
	}
}

void ApplicationLayer::OnEvent(Coil::Event& event)
{
	CL_PROFILE_FUNCTION_HIGH()

	CameraController.OnEvent(event);

	Coil::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Coil::KeyPressedEvent>(BIND_EVENT_METHOD(ApplicationLayer::OnKeyPressed));
	dispatcher.Dispatch<Coil::KeyReleasedEvent>(BIND_EVENT_METHOD(ApplicationLayer::OnKeyReleased));
}

bool ApplicationLayer::OnKeyPressed(Coil::KeyPressedEvent& event) const
{
	CL_PROFILE_FUNCTION_HIGH()

	if (event.GetKeyCode() == CL_KEY_P && event.GetRepeatCount() == 0)
	{
		if (Coil::Input::IsKeyPressed(CL_KEY_LEFT_CONTROL))
		{
			auto timestamp = Coil::Time::NowString();
			timestamp->Replace(':', '_');
			CL_PROFILE_BEGIN_SESSION_LOW("Low Profiling", Coil::PString("Profiling/CoilProfileLow-Runtime %R.json", &timestamp));
		}
		else
		{
			auto timestamp = Coil::Time::NowString();
			timestamp->Replace(':', '_');
			CL_PROFILE_BEGIN_SESSION_HIGH("High Profiling", Coil::PString("Profiling/CoilProfileHigh-Runtime %R.json", &timestamp));
		}
	}

	return false;
}

bool ApplicationLayer::OnKeyReleased(Coil::KeyReleasedEvent& event) const
{
	CL_PROFILE_FUNCTION_HIGH()

	if (event.GetKeyCode() == CL_KEY_P)
		CL_PROFILE_END_SESSION()

	return false;
}

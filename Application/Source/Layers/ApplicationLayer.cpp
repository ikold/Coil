#include "ApplicationLayer.h"

#include <glm/ext/scalar_constants.hpp>


ApplicationLayer::ApplicationLayer()
	: Layer("Application"),
	  CameraController(1280.0f / 720.0f, true),
	  FrameTime(Coil::PString("%{#frame time.3}16f ms", 0)),
	  MousePosition(Coil::PString("mouse x: %{#X}6d y: %{#Y}6d", 0, 0)),
	  SquareColor(Coil::CreateRef<glm::vec4>(0.2f, 0.2f, 0.2, 1.f)),
	  TimerString(Coil::PString("%{.2}8f seconds remains", 0))
{
	CL_PROFILE_FUNCTION_HIGH()

	Coil::Application::Get().GetWindow().SetVSync(false);

	// Creating and binding Log window
	Coil::GUI::LogWindow({ "Log" })->BindBuffer(Coil::Logger::GetBuffer());

	// Creating overlay for displaying frame time
	Coil::GUI::Overlay({ "frame time" })->BindTextBuffer(FrameTime);

	Coil::GUI::ComponentWindow({ "Name" }, {
		Coil::GUI::Text("Square Color"),
		// Allows for changing color of squares in the background
		Coil::GUI::ColorPicker({ "", -1 }, SquareColor),
		// Will profile next five seconds on High level 
		Coil::GUI::Button({ "Profile (High) 5 seconds" }, [&]
		{
			Timer     = 5.f;
			auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
			CL_PROFILE_BEGIN_SESSION_HIGH("High Profiling", Coil::PString("Profiling/CoilProfileHigh5s-Runtime %R.json", &time));
		}),
		// Will profile next five seconds on Low level 
		Coil::GUI::Button({ "Profile (Low) 5 seconds" }, [&]
		{
			Timer     = 5.f;
			auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
			CL_PROFILE_BEGIN_SESSION_LOW("High Profiling", Coil::PString("Profiling/CoilProfileLow5s-Runtime %R.json", &time));
		})
	});

	// Display dynamically updated mouse position
	Coil::Logger::Trace(MousePosition);
	Coil::Logger::Trace(TimerString);

	Texture = Coil::Texture2D::Create("Resources/Textures/Colorgrid.png");
}

void ApplicationLayer::OnUpdate()
{
	CL_PROFILE_FUNCTION_HIGH()

	// Updates frame time overlay
	FrameTime->Set("frame time", Coil::Time::DeltaTime());
	FrameTime->SetIndex(0, Coil::Time::DeltaTime());

	CameraController.OnUpdate();

	// Timer for the profiling
	if (Timer > 0.f)
	{
		Timer -= Coil::Time::DeltaTime() / 1000.f;

		if (Timer <= 0.f)
		{
			Timer = 0.f;
			CL_PROFILE_END_SESSION()
		}
	}

	// Updates mouse position log
	auto [mouseX, mouseY] = Coil::Input::GetMousePosition();
	MousePosition->Set("X", static_cast<int64>(mouseX));
	MousePosition->Set("Y", static_cast<int64>(mouseY));

	{
		CL_PROFILE_SCOPE_HIGH("Rendering")

		Coil::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Coil::RenderCommand::Clear();

		Coil::Renderer2D::BeginScene(CameraController.GetCamera());

		const int32 gridHeight = 200;
		const int32 gridWidth  = 40;

		// Increments iterator by half of frame time in seconds
		TimeIteration += Coil::Time::DeltaTime() / 1000.f * 0.5f;

		// Keeps iterator in range (0.f, 4.f>
		while (TimeIteration > 4.f)
			TimeIteration -= 4.f;

		// Draws main square
		Coil::Renderer2D::DrawQuad(glm::vec3(0.f), TimeIteration / 2 * glm::pi<float32>(), glm::vec2(1.f), Texture);

		{
			CL_PROFILE_SCOPE_HIGH("Grid rendering")

			// Draws background squares offset by TimeIteration to simulate endlessly moving background
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

	// Passes the event to camera controller
	CameraController.OnEvent(event);

	Coil::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Coil::KeyPressedEvent>(BIND_EVENT_METHOD(ApplicationLayer::OnKeyPressed));
	dispatcher.Dispatch<Coil::KeyReleasedEvent>(BIND_EVENT_METHOD(ApplicationLayer::OnKeyReleased));
}

bool ApplicationLayer::OnKeyPressed(Coil::KeyPressedEvent& event) const
{
	CL_PROFILE_FUNCTION_HIGH()

	// Starts profiling High profiling for P key and Low profiling for Ctrl + P key
	if (event.GetKeyCode() == CL_KEY_P && event.GetRepeatCount() == 0)
	{
		if (Coil::Input::IsKeyPressed(CL_KEY_LEFT_CONTROL))
		{
			auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
			CL_PROFILE_BEGIN_SESSION_LOW("Low Profiling", Coil::PString("Profiling/CoilProfileLow-Runtime %R.json", &time));
		}
		else
		{
			auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
			CL_PROFILE_BEGIN_SESSION_HIGH("High Profiling", Coil::PString("Profiling/CoilProfileHigh-Runtime %R.json", &time));
		}
	}

	return false;
}

bool ApplicationLayer::OnKeyReleased(Coil::KeyReleasedEvent& event) const
{
	CL_PROFILE_FUNCTION_HIGH()

	// Stops profiling
	if (event.GetKeyCode() == CL_KEY_P)
		CL_PROFILE_END_SESSION()

	return false;
}

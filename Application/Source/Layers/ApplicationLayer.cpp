#include "ApplicationLayer.h"

#include <glm/ext/scalar_constants.hpp>


ApplicationLayer::ApplicationLayer()
	: Layer("Application"),
	CameraController(1280.0f / 720.0f, true),
	FrameTime(Coil::PString("%{#frame time.3}16f ms", 0)),
	MousePosition(Coil::PString("mouse x: %{#X}6d y: %{#Y}6d", 0, 0)),
	SquareColor(Coil::CreateRef<glm::vec4>(0.2f, 0.2f, 0.2, 1.f)),
	RendererStatisticsString(Coil::PString("Draw Calls: %{#draw calls}d\nQuads: %{#quads}d\nVertices: %{#vertices}d\nIndices: %{#indices}d\nMemory Used: %8{#memory used.1}f %{#memory unit}16s", 0, 0, 0, 0, 0, 0)),
	InstrumentorStatisticsString(Coil::PString("Profiling %16{#level}s\nProfiles: %{#profiles}d\nMemory Used/Reserved: %8{#memory used.1}f/%8{#memory reserved.1}f %{#memory unit}16s\nElapsed Time: %{#elapsed time}f ms", 0, 0, 0, 0, 0, 0))
{
	CL_PROFILE_FUNCTION_HIGH()

		Coil::Application::Get().GetWindow().SetVSync(true);

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
			auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
			CL_PROFILE_BEGIN_SESSION_HIGH("High Profiling", Coil::PString("Profiling/CoilProfileHigh5s-Runtime %R.json", &time));

			CL_PROFILE_END_SESSION_TIMER(5000)
		}),
		// Will profile next five seconds on Medium level 
		Coil::GUI::Button({ "Profile (Medium) 5 seconds" }, [&]
		{
			auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
			CL_PROFILE_BEGIN_SESSION_MEDIUM("High Profiling", Coil::PString("Profiling/CoilProfileMedium5s-Runtime %R.json", &time));

			CL_PROFILE_END_SESSION_TIMER(5000)
		}),
				// Will profile next five seconds on Low level 
				Coil::GUI::Button({ "Profile (Low) 5 seconds" }, [&]
				{
					auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
					CL_PROFILE_BEGIN_SESSION_LOW("High Profiling", Coil::PString("Profiling/CoilProfileLow5s-Runtime %R.json", &time));

					CL_PROFILE_END_SESSION_TIMER(5000)
				})
		});


	Coil::GUI::ComponentWindow({ "Renderer2D Statistics" }, {
		Coil::GUI::Text(RendererStatisticsString)
		});


	Coil::GUI::ComponentWindow({ "Profiling Statistics" }, {
		Coil::GUI::Text(InstrumentorStatisticsString)
		});


	// Display dynamically updated mouse position
	Coil::Logger::Trace(MousePosition);

	Texture = Coil::Texture2D::Create("Resources/Textures/Colorgrid.png");
}

void ApplicationLayer::OnUpdate()
{
	CL_PROFILE_FUNCTION_HIGH()

		// Updates frame time overlay
		FrameTime->Set("frame time", Coil::Time::DeltaTime());
	FrameTime->SetIndex(0, Coil::Time::DeltaTime());

	CameraController.OnUpdate();

	// Updates mouse position log
	auto [mouseX, mouseY] = Coil::Input::GetMousePosition();
	MousePosition->Set("X", static_cast<int64>(mouseX));
	MousePosition->Set("Y", static_cast<int64>(mouseY));

	{
		{
			CL_PROFILE_SCOPE_HIGH("Rendering")

				Coil::Renderer2D::ResetStatistics();

			Coil::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			Coil::RenderCommand::Clear();

			Coil::Renderer2D::BeginScene(CameraController.GetCamera());

			const int32 gridHeight = 40;
			const int32 gridWidth = 60;

			// Increments iterator by half of frame time in seconds
			TimeIteration += Coil::Time::DeltaTime() / 1000.f * 0.5f;

			// Keeps iterator in range (0.f, 4.f>
			while (TimeIteration > 4.f)
				TimeIteration -= 4.f;

			Coil::Renderer2D::DrawQuad({ 0.f, 0.f, 0.1f }, glm::vec2(1.f), Texture, { 1.f, 1.f, 1.f, 1.f });

			{
				CL_PROFILE_SCOPE_HIGH("Grid rendering")

					auto quadBuilder = Coil::Renderer2D::QuadBuilder();

				quadBuilder.SetColor(*SquareColor);
				quadBuilder.SetScale({ 0.1f, 0.1f });
				quadBuilder.SetRotation(TimeIteration / 2 * glm::pi<float32>());

				// Draws background squares offset by TimeIteration to simulate endlessly moving background
				for (int32 x = 0; x < gridWidth; ++x)
				{
					CL_PROFILE_SCOPE_LOW("Draw collumn")

						quadBuilder.SetPosition({
							(-gridWidth / 2.f - TimeIteration + x) * 0.11f,
							(-gridHeight / 2.f + TimeIteration / 4.f) * 0.11f
							});

					for (int32 y = 0; y < gridHeight; ++y)
					{
						CL_PROFILE_SCOPE_LOW("Draw cell")

							quadBuilder.Draw();

						quadBuilder.Move({ 0.f, 0.11f });
					}
				}
			}

			Coil::Renderer2D::EndScene();
		}

		{
			CL_PROFILE_SCOPE_HIGH("Rendering stats")

				const auto renderingStats = Coil::Renderer2D::GetStatistics();

			RendererStatisticsString->Set("draw calls", renderingStats.DrawCalls);
			RendererStatisticsString->Set("quads", renderingStats.QuadCount);
			RendererStatisticsString->Set("vertices", renderingStats.GetTotalVertexCount());
			RendererStatisticsString->Set("indices", renderingStats.GetTotalIndexCount());

			float32 usedMemory = static_cast<float32>(renderingStats.QuadCount) * Coil::Renderer2D::GetSizeOfQuad();

			RendererStatisticsString->Set("memory unit", "Bytes");
			if (usedMemory / 1024.f > 1.f)
			{
				usedMemory /= 1024.f;
				RendererStatisticsString->Set("memory unit", "KB");
			}

			if (usedMemory / 1024.f > 1.f)
			{
				usedMemory /= 1024.f;
				RendererStatisticsString->Set("memory unit", "MB");
			}

			if (usedMemory / 1024.f > 1.f)
			{
				usedMemory /= 1024.f;
				RendererStatisticsString->Set("memory unit", "GB");
			}


			RendererStatisticsString->Set("memory used", usedMemory);
		}

		{
			CL_PROFILE_SCOPE_HIGH("Profiling stats")

				const auto profilingStats = Coil::Instrumentor::GetStatistics();

			InstrumentorStatisticsString->Set("level", Coil::Instrumentor::ProfilingLevelToString(profilingStats.ProfilingLevel)->CString());
			InstrumentorStatisticsString->Set("profiles", profilingStats.NumberOfProfiles);

			auto usedMemory = static_cast<float32>(profilingStats.MemoryUsed);
			auto reservedMemory = static_cast<float32>(profilingStats.MemoryReserved);

			InstrumentorStatisticsString->Set("memory unit", "Bytes");
			if (usedMemory / 1024.f > 1.f)
			{
				usedMemory /= 1024.f;
				reservedMemory /= 1024.f;
				InstrumentorStatisticsString->Set("memory unit", "KB");
			}

			if (usedMemory / 1024.f > 1.f)
			{
				usedMemory /= 1024.f;
				reservedMemory /= 1024.f;
				InstrumentorStatisticsString->Set("memory unit", "MB");
			}

			if (usedMemory / 1024.f > 1.f)
			{
				usedMemory /= 1024.f;
				reservedMemory /= 1024.f;
				InstrumentorStatisticsString->Set("memory unit", "GB");
			}


			InstrumentorStatisticsString->Set("memory used", usedMemory);
			InstrumentorStatisticsString->Set("memory reserved", reservedMemory);

			InstrumentorStatisticsString->Set("elapsed time", Coil::Time::QueryConverter<float32>(Coil::Time::Unit::Millisecond)(profilingStats.ElapsedTime));
		}
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

		// Starts profiling High profiling for P key, Medium profiling for Shift + P key and Low profiling for Ctrl + P key
		if (event.GetKeyCode() == CL_KEY_P && event.GetRepeatCount() == 0)
		{
			if (Coil::Input::IsKeyPressed(CL_KEY_LEFT_CONTROL))
			{
				auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
				CL_PROFILE_BEGIN_SESSION_LOW("Low Profiling", Coil::PString("Profiling/CoilProfileLow-Runtime %R.json", &time));
			}
			else if (Coil::Input::IsKeyPressed(CL_KEY_LEFT_SHIFT))
			{
				auto time = Coil::Time::NowString("%Y-%m-%d %H_%M_%S");
				CL_PROFILE_BEGIN_SESSION_MEDIUM("Medium Profiling", Coil::PString("Profiling/CoilProfileMedium-Runtime %R.json", &time));
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

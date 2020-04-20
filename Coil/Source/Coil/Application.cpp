#include "pch.h"
#include "Application.h"

#include "Coil/ImGui/ImGuiInterface.h"
#include "Coil/ImGui/ImGuiLogWindow.h"
#include "Coil/ImGui/ImGuiOverlay.h"

#include "Coil/ImGui/ImGuiLayer.h"

#include <glad/glad.h>

#include "Coil/Input.h"

namespace Coil
{
	Application* Application::Instance = nullptr;

	Application::Application()
		:Running(true)
	{
		CL_ASSERT(!Instance, "Application instance already exist!")
		Instance = this;

		AppWindow = std::unique_ptr<Window>(Window::Create());
		AppWindow->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		AppWindow->SetVSync(true);

		ImGuiInterface::Creat<ImGuiLogWindow>("Log").BindBuffer(Logger::GetBuffer());
		PushOverlay(new ImGuiLayer());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Logger::Info("Initializating Application");
		
		Logger::Info("Running Application");

		RString frameTime = PString("%f ms", 0.);
		ImGuiInterface::Creat<ImGuiOverlay>("frame time").BindTextBuffer(frameTime);

		RString mousePosition = PString("x: %d y: %d", 0, 0);
		ImGuiInterface::Creat<ImGuiOverlay>("mouse position").BindTextBuffer(mousePosition);

		int32 counter = 0;
		float32 frameTimeArray[60];

		while (Running)
		{
			// computing of frame time
			Time::Tick();

			if (!Input::IsKeyPressed(CL_KEY_TAB))
			{
				auto [x, y] = Input::GetMousePosition();
				mousePosition.Get<PString>()->Set(0, (int32)x);
				mousePosition.Get<PString>()->Set(1, (int32)y);
			}


			frameTimeArray[counter] = Time::DeltaTime();

			if (++counter > 59)
			{
				counter = 0;
				float32 sum = 0.f;
				for (int32 i = 0; i < 60; ++i)
					sum += frameTimeArray[i];

				frameTime.Get<PString>()->Set(0, sum / 60);
			}
			
			Update();
		}
	}

	void Application::Update()
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto layer : AppLayerStack)
			layer->OnUpdate();

		ImGuiLayer::Begin();
		ImGuiInterface::OnImGuiRender();

		for (Layer* layer : AppLayerStack)
			layer->OnImGuiRender();
		ImGuiLayer::End();

		AppWindow->OnUpdate();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_METHOD(Application::OnWindowClosed));

		for (auto it = AppLayerStack.end(); it != AppLayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.IsHandled())
				break;
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& event)
	{
		Logger::Info("Closing Application");
		Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		AppLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		AppLayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
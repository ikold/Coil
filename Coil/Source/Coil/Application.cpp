#include "pch.h"
#include "Application.h"

#include "Coil/ImGui/ImGuiInterface.h"
#include "Coil/ImGui/ImGuiLogWindow.h"
#include "Coil/ImGui/ImGuiOverlay.h"

#include <glad/glad.h>

#include "Coil/Utilities/String.h"


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
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Logger::Info("Initializating Application");
		
		Logger::Info("Running Application");

		std::string frameTime = "test";
		std::stringstream ss;

		ImGuiInterface::Creat<ImGuiOverlay>("frame time").BindTextBuffer(&frameTime);

		int counter = 0;
		float frameTimeArray[60];


		while (Running)
		{
			// computing of frame time
			Time::Tick();

			ss.str(std::string());

			frameTimeArray[counter] = Time::DeltaTime();

			if (++counter > 59)
			{
				counter = 0;
				float sum = 0.f;
				for (int i = 0; i < 60; ++i)
					sum += frameTimeArray[i];

				ss << (sum / 60)<< "ms";

				frameTime = ss.str();
			}

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			
			for (auto layer : AppLayerStack)
				layer->OnUpdate();

			AppWindow->OnUpdate();
		}
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
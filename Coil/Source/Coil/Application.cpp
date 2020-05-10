#include "pch.h"
#include "Application.h"

#include "Coil/ImGui/ImGuiLayer.h"
#include "Coil/ImGui/ImGuiInterface.h"


namespace Coil
{
	Application* Application::Instance = nullptr;

	Application::Application()
		: Running(true)
	{
		CL_ASSERT(!Instance, "Application instance already exist!")
		Instance = this;

		AppWindow = std::unique_ptr<Window>(Window::Create());
		AppWindow->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		PushOverlay(new ImGuiLayer());
	}

	void Application::Run()
	{
		Logger::Info("Running Application");

		while (Running)
		{
			// computing of frame time
			Time::Tick();

			for (auto* layer : AppLayerStack)
				layer->OnUpdate();

			ImGuiLayer::Begin();
			ImGuiInterface::OnImGuiRender();

			for (auto* layer : AppLayerStack)
				layer->OnImGuiRender();
			ImGuiLayer::End();

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

	bool Application::OnWindowClosed([[maybe_unused]] WindowCloseEvent& event)
	{
		Logger::Info("Closing Application");
		Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		AppLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		AppLayerStack.PushOverlay(overlay);
	}
}

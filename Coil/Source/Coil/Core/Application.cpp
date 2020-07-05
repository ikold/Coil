#include "pch.h"
#include "Application.h"

#include "Coil/GUI/GUILayer.h"

#include "Coil/Renderer/Renderer.h"


namespace Coil
{
	Application* Application::Instance = nullptr;

	Application::Application()
		: Running(true)
	{
		CL_PROFILE_FUNCTION()

		CL_ASSERT(!Instance, "Application instance already exist!")
		Instance = this;

		AppWindow = Window::Create();
		AppWindow->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		Renderer::Init();

		PushOverlay(new GUILayer());
	}

	Application::~Application()
	{
		CL_PROFILE_FUNCTION()

		Renderer::ShutDown();
	}

	void Application::Run()
	{
		CL_PROFILE_FUNCTION()

		Logger::Info("Running Application");

		while (Running)
		{
			CL_PROFILE_SCOPE("Main Loop")
			// computing of frame time
			Time::Tick();

			if (!Minimized)
			{
				CL_PROFILE_SCOPE("LayerStack OnUpdate")

				for (auto* layer : AppLayerStack)
					layer->OnUpdate();
			}

			{
				CL_PROFILE_SCOPE("LayerStack OnImGuiRender")

				for (auto* layer : AppLayerStack)
					layer->OnImGuiRender();
			}

			AppWindow->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		CL_PROFILE_FUNCTION()

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_METHOD(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_METHOD(Application::OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		CL_PROFILE_FUNCTION()

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			Minimized = true;
			return false;
		}

		Minimized = false;

		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		CL_PROFILE_FUNCTION()

		AppLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		CL_PROFILE_FUNCTION()

		AppLayerStack.PushOverlay(overlay);
	}
}

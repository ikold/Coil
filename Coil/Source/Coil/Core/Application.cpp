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
		CL_PROFILE_FUNCTION_HIGH()

		CL_ASSERT(!Instance, "Application instance already exist!")
		Instance = this;

		AppWindow = Window::Create();
		AppWindow->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		Renderer::Init();

		PushOverlay(new GUILayer());
	}

	Application::~Application()
	{
		CL_PROFILE_FUNCTION_HIGH()

		Renderer::ShutDown();
	}

	void Application::Run()
	{
		CL_PROFILE_FUNCTION_HIGH()

		Logger::Info("Running Application");

		while (Running)
		{
			CL_PROFILE_SCOPE_HIGH("Main Loop")

			// Computing frame time
			Time::Tick();

			if (!Minimized)
			{
				CL_PROFILE_SCOPE_HIGH("AppLayerStack->OnUpdate()")

				for (auto* layer : AppLayerStack)
					layer->OnUpdate();
			}

			{
				CL_PROFILE_SCOPE_HIGH("AppLayerStack->OnImGuiRender()")

				for (auto* layer : AppLayerStack)
					layer->OnImGuiRender();
			}

			AppWindow->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		CL_PROFILE_FUNCTION_HIGH()

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
		CL_PROFILE_FUNCTION_HIGH()

		Logger::Info("Closing Application");
		Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		CL_PROFILE_FUNCTION_HIGH()

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
		CL_PROFILE_FUNCTION_HIGH()

		AppLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		CL_PROFILE_FUNCTION_HIGH()

		AppLayerStack.PushOverlay(overlay);
	}
}

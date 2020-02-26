#include "pch.h"
#include "Application.h"


namespace Coil
{
	Application::Application()
		:Running(true)
	{
		AppWindow = std::unique_ptr<Window>(Window::Create());
		AppWindow->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Logger::Info("Initializating Application");
		
		
		Logger::Debug("Running Application");
		
		while (Running)
		{
			// computing of frame time
			Time::Tick();
			
			AppWindow->OnUpdate();

			// tmp
			//Running = false;
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_METHOD(Application::OnWindowClosed));

		Logger::Trace(e.ToString().c_str());
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		Running = false;
		return true;
	}
}
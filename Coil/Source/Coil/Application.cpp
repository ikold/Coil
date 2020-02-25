#include "pch.h"
#include "Application.h"


namespace Coil
{
	Application::Application()
		:Running(true)
	{
		AppWindow = std::unique_ptr<Window>(Window::Create());
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
			Running = false;
		}
	}
}
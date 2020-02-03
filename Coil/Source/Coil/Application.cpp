#include "pch.h"
#include "Application.h"

#include "Time.h"
#include "Logger.h"


namespace Coil
{
	Application::Application()
		:Running(true)
	{
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
			
			// tmp
			Running = false;
		}
	}
}
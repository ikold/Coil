#include "pch.h"
#include "Application.h"

#include "Coil/Time.h"
#include "Coil/Logger.h"


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
			Time::Tick();
			
			Running = false;
		}
	}
}
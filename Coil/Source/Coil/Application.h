#pragma once

#include "Coil/Core.h"

namespace Coil
{
	class COIL_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
		
	private:
		bool Running;
	};

	Application* CreatApplication();
}


/*!	Creates declaration of Coil::CreatApplication()

	@param app - child class of Coil::Aplication
	- Use to bind client application with the engine
*/
#define BindApplication(app) Coil::Application* Coil::CreatApplication() { return new app; }
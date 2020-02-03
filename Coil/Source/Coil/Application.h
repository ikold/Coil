#pragma once

#include "Coil/Core.h"

namespace Coil
{
	/*!	Class for application and it functionality
	*/
	class COIL_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		/*!	Main loop of the application
		*/
		void Run();
		
	private:
		bool Running;	/*!	Flag responsible of running of main loop */
	};


	/*!	Method for creating new application
		Should be defined by BindApplication(app)
	*/
	Application* CreatApplication();
}


/*!	Creates declaration of Coil::CreatApplication()
	Use to bind client application with the engine
	
	@param app	child class of Coil::Aplication
*/
#define BindApplication(app) Coil::Application* Coil::CreatApplication() { return new app; }
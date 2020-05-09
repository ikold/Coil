#pragma once

#include "Coil/Core.h"

#include "Coil/Layers/LayerStack.h"
#include "Coil/Window.h"

namespace Coil
{
	/*!	Class for application and it functionality
	*/
	class Application
	{
	public:
		Application();
		virtual ~Application() {};

		/*!	Main loop of the application
		*/
		void Run();

		void Update();

		void OnEvent(Event& event);

		bool OnWindowClosed(WindowCloseEvent& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *AppWindow; }

		static inline Application& Get() { return *Instance; }
	private:
		bool Running;	/*!	Flag responsible of running of main loop */

		std::unique_ptr<Window> AppWindow;

		LayerStack AppLayerStack;

		static Application* Instance;
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
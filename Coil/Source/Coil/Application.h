#pragma once

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
		virtual ~Application() = default;

		/*!	Main loop of the application
		*/
		void Run();

		void OnEvent(Event& event);

		bool OnWindowClosed(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		[[nodiscard]] Window& GetWindow() const { return *AppWindow; }

		static Application& Get() { return *Instance; }

	private:
		bool Running;	/*!	Flag responsible of running of main loop */
		
		bool Minimized = false;
		
		std::unique_ptr<Window> AppWindow;

		LayerStack AppLayerStack;

		static Application* Instance;
	};


	/*!	Method for creating new application
		Should be defined by BindApplication(app)
	*/
	Application* CreateApplication();
}


/*!	Creates declaration of Coil::CreateApplication()
	Use to bind client application with the engine

	@param app	child class of Coil::Application
*/
#define BindApplication(app) Coil::Application* Coil::CreateApplication() { return new (app); }

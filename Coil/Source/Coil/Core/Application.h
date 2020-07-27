#pragma once

#include "Coil/Core/Layer/LayerStack.h"
#include "Coil/Core/Window.h"


namespace Coil
{
	/**
	 * @brief Class for application and it functionality
	 */
	class Application
	{
	public:
		Application();

		Application(const Application& rhs) = delete;
		Application(Application&& rhs) = delete;

		virtual ~Application();

		Application& operator=(const Application& rhs) = delete;
		Application& operator=(Application&& rhs) = delete;


		/**
		 * @brief Main loop of the application
		 */
		void Run();

		void OnEvent(Event& event);

		bool OnWindowResize(WindowResizeEvent& event);
		bool OnWindowClosed(WindowCloseEvent& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		[[nodiscard]] Window& GetWindow() const { return *AppWindow; }

		static Application& Get() { return *Instance; }

	private:
		/** Flag for main loop */
		bool Running;

		bool Minimized = false;

		Scope<Window> AppWindow;

		LayerStack AppLayerStack;

		static Application* Instance;
	};


	/**
	 * @brief Method for creating new application
	 *
	 * @note Should be defined by BindApplication(app)
	 */
	Application* CreateApplication();
}


/**
 * @brief Creates declaration of Coil::CreateApplication()
 *
 * @param app	child class of Coil::Application
 *
 * @note Use to bind client application with the engine
 */
#define BindApplication(app) Coil::Application* Coil::CreateApplication() { return new (app); }

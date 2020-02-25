#pragma once

#include "Coil/Window.h"

#include <GLFW/glfw3.h>

namespace Coil
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return Data.Width; }
		inline unsigned int GetHeight() const override { return Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { Data.EventCallback = callback; }

		void SetVSync(bool enable = true) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* Window;

		struct WindowData
		{
			std::string Name;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData Data;
	};
}
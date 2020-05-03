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

		inline uint32 GetWidth() const override { return Data.Width; }
		inline uint32 GetHeight() const override { return Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { Data.EventCallback = callback; }

		void SetVSync(bool enable = true) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return WindowInstance; };

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* WindowInstance;

		struct WindowData
		{
			RString<String> Name;
			uint32 Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData Data;
	};
}
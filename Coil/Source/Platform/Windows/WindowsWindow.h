#pragma once

#include "Coil/Window.h"

#include "Coil/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>


namespace Coil
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		[[nodiscard]] uint32 GetWidth() const override { return Data.Width; }
		[[nodiscard]] uint32 GetHeight() const override { return Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { Data.EventCallback = callback; }

		void SetVSync(bool enable = true) override;
		[[nodiscard]] bool IsVSync() const override;

		[[nodiscard]] void* GetNativeWindow() const override { return WindowInstance; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* WindowInstance{};
		GraphicsContext* Context{};


		struct WindowData
		{
			RString<> Name;
			uint32 Width{}, Height{};
			bool VSync{};

			EventCallbackFn EventCallback;
		};


		WindowData Data;
	};
}

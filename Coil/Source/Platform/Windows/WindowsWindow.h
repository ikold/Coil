#pragma once

#include "Coil/Window.h"

#include "Coil/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>


namespace Coil
{
	class WindowsWindow final : public Window
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		
		~WindowsWindow() override;

		void OnUpdate() override;

		[[nodiscard]] uint32 GetWidth() const override { return Data.Width; }
		[[nodiscard]] uint32 GetHeight() const override { return Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { Data.EventCallback = callback; }

		void SetVSync(bool enable = true) override;
		[[nodiscard]] bool IsVSync() const override;

		[[nodiscard]] void* GetNativeWindow() const override { return WindowInstance; }

	private:
		void Init(const WindowProps& props);
		void Shutdown() const;

	private:
		GLFWwindow* WindowInstance{};
		Scope<GraphicsContext> Context{};


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

#pragma once


namespace Coil
{
	struct WindowProps
	{
		RString<> Name = "Coil Engine";
		uint32 Width   = 1280;
		uint32 Height  = 720;
	};


	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window()                  = default;
		Window(const Window& rhs) = default;
		Window(Window&& rhs)      = default;

		virtual ~Window() = default;

		Window& operator=(const Window& rhs) = default;
		Window& operator=(Window&& rhs)      = default;

		virtual void OnUpdate() = 0;

		[[nodiscard]] virtual uint32 GetWidth() const = 0;
		[[nodiscard]] virtual uint32 GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		[[nodiscard]] virtual bool IsVSync() const = 0;

		[[nodiscard]] virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}

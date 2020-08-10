#pragma once


namespace Coil
{
	struct WindowProps
	{
		RString<> Name = "Coil Engine";
		uint32 Width   = 1280;
		uint32 Height  = 720;
	};


	/**
	 * @brief Base Window class
	 *
	 * @note Should have one derive per platform
	 */
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

		/**
		 * @brief Interface for getting Window Width
		 *
		 * @return		Width in pixels
		 */
		[[nodiscard]] virtual uint32 GetWidth() const = 0;
		/**
		 * @brief Interface for getting Window Height
		 *
		 * @return		Height in pixels
		 */
		[[nodiscard]] virtual uint32 GetHeight() const = 0;

		/**
		 * @brief Binds window events with application callback function
		 *
		 * @param[in]	callback	Application callback function
		 */
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		[[nodiscard]] virtual bool IsVSync() const = 0;

		/**
		 * @brief Interface for getting native window
		 *
		 * @return		Platform native window
		 *
		 * @note returned native window is not child class of this window class (e.g. GLFWwindow*)
		 */
		[[nodiscard]] virtual void* GetNativeWindow() const = 0;

		/**
		 * @brief Create method for platform specific window
		 *
		 * @param[in]	props	Window properties
		 *
		 * @return		Scope to the native window
		 *
		 * @note Defined in platform specific window cpp file
		 */
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}

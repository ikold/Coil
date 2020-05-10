#include "pch.h"
#include "WindowsWindow.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Coil
{
	static bool GLFWInitialized = false;

	static void GLFWErrorCallback([[maybe_unused]] int32 error, const char* description)
	{
		Logger::Error(description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		WindowsWindow::Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		WindowsWindow::Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		Data.Name = props.Name;
		Data.Width = props.Width;
		Data.Height = props.Height;

		Data.VSync = true;

		Logger::Info("Creating window");

		if (!GLFWInitialized)
		{
			const int32 success = glfwInit();

			CL_CORE_ASSERT(success, "GLFW initialization failed!");

			glfwSetErrorCallback(GLFWErrorCallback);

			GLFWInitialized = true;
		}

		WindowInstance = glfwCreateWindow(static_cast<int32>(Data.Width), static_cast<int32>(Data.Height), Data.Name->CString(), nullptr, nullptr);


		Context = new OpenGLContext(WindowInstance);
		Context->Init();

		glfwSetWindowUserPointer(WindowInstance, &Data);
		SetVSync(Data.VSync);

		glfwSetWindowSizeCallback(WindowInstance, [](GLFWwindow* window, int32 width, int32 height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data.Width = static_cast<uint32>(width);
			data.Height = static_cast<uint32>(height);

			WindowResizeEvent event(static_cast<uint32>(width), static_cast<uint32>(height));
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(WindowInstance, [](GLFWwindow* window)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		static int32 keyRepeatCounter = 0;

		glfwSetKeyCallback(WindowInstance, [](GLFWwindow* window, int32 key, [[maybe_unused]] int32 scancode, int32 action, [[maybe_unused]] int32 mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, keyRepeatCounter = 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, ++keyRepeatCounter);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(WindowInstance, [](GLFWwindow* window, uint32 keycode)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(WindowInstance, [](GLFWwindow* window, int32 button, int32 action, [[maybe_unused]] int32 mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(WindowInstance, [](GLFWwindow* window, float64 xOffset, float64 yOffset)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(static_cast<float32>(xOffset), static_cast<float32>(yOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(WindowInstance, [](GLFWwindow* window, float64 xPos, float64 yPos)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(static_cast<float32>(xPos), static_cast<float32>(yPos));
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(WindowInstance);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		glfwSwapInterval(static_cast<int32>(enable));

		Data.VSync = enable;
	}

	bool WindowsWindow::IsVSync() const
	{
		return Data.VSync;
	}
}
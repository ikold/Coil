#include "pch.h"
#include "WindowsWindow.h"

#include "glad/glad.h"


namespace Coil
{
	static bool GLFWInitializated = false;

	static void GLFWErrorCallback(int32 error, const char* description)
	{
		Logger::Error(description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		Data.Name = props.Name;
		Data.Width = props.Width;
		Data.Height = props.Height;

		Data.VSync = true;

		Logger::Info("Creating window");

		if (!GLFWInitializated)
		{
			int32 success = glfwInit();

			CL_CORE_ASSERT(success, "GLFW initialization failed!");

			glfwSetErrorCallback(GLFWErrorCallback);

			GLFWInitializated = true;
		}

		WindowInstance = glfwCreateWindow((int32)Data.Width, (int32)Data.Height, Data.Name->CString(), nullptr, nullptr);
		glfwMakeContextCurrent(WindowInstance);
		int32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CL_CORE_ASSERT(status, "GLAD initialization failed!");
		glfwSetWindowUserPointer(WindowInstance, &Data);
		SetVSync(Data.VSync);

		glfwSetWindowSizeCallback(WindowInstance, [](GLFWwindow* window, int32 width, int32 height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(WindowInstance, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		static int32 KeyRepeatCounter = 0;

		glfwSetKeyCallback(WindowInstance, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
			case GLFW_PRESS:
				{
					KeyPressedEvent event(key, KeyRepeatCounter = 0);
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
					KeyPressedEvent event(key, ++KeyRepeatCounter);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(WindowInstance, [](GLFWwindow* window, uint32 keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(WindowInstance, [](GLFWwindow* window, int32 button, int32 action, int32 mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

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
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float32)xOffset, (float32)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(WindowInstance, [](GLFWwindow* window, float64 xPos, float64 yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float32)xPos, (float32)yPos);
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
		glfwSwapBuffers(WindowInstance);
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		glfwSwapInterval((int32)enable);

		Data.VSync = enable;
	}

	bool WindowsWindow::IsVSync() const
	{
		return Data.VSync;
	}
}
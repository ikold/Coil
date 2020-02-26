#include "pch.h"
#include "WindowsWindow.h"

namespace Coil
{
	static bool GLFWInitializated = false;

	static void GLFWErrorCallback(int error, const char* description)
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
			int success = glfwInit();

			CL_CORE_ASSERT(success, "GLFW initialization failed!");

			glfwSetErrorCallback(GLFWErrorCallback);

			GLFWInitializated = true;
		}

		WindowInstance = glfwCreateWindow((int)Data.Width, (int)Data.Height, Data.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(WindowInstance);
		glfwSetWindowUserPointer(WindowInstance, &Data);
		SetVSync(Data.VSync);

		glfwSetWindowSizeCallback(WindowInstance, [](GLFWwindow* window, int width, int height)
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

		static int KeyRepeatCounter = 0;

		glfwSetKeyCallback(WindowInstance, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

		glfwSetMouseButtonCallback(WindowInstance, [](GLFWwindow* window, int button, int action, int mods)
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

		glfwSetScrollCallback(WindowInstance, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrollEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(WindowInstance, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
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
		glfwSwapInterval((int)enable);

		Data.VSync = enable;
	}

	bool WindowsWindow::IsVSync() const
	{
		return Data.VSync;
	}
}
#include "pch.h"
#include "WindowsWindow.h"

namespace Coil
{
	static bool GLFWInitializated = false;

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

			GLFWInitializated = true;
		}

		Window = glfwCreateWindow((int)Data.Width, (int)Data.Height, Data.Name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(Window);
		glfwSetWindowUserPointer(Window, &Data);
		SetVSync(Data.VSync);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(Window);
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
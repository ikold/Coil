#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Coil
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: WindowHandle(windowHandle)
	{
		CL_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	OpenGLContext::~OpenGLContext()
	{}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(WindowHandle);
		int32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CL_CORE_ASSERT(status, "GLAD initialization failed!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(WindowHandle);
	}
}
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

	void OpenGLContext::Init()
	{
		CL_PROFILE_FUNCTION_HIGH()

		glfwMakeContextCurrent(WindowHandle);
		const int32 status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		CL_CORE_ASSERT(status, "GLAD initialization failed!");

		const RString<> vendor   = String(reinterpret_cast<const char8*>(glGetString(GL_VENDOR)));
		const RString<> renderer = String(reinterpret_cast<const char8*>(glGetString(GL_RENDERER)));
		const RString<> version  = String(reinterpret_cast<const char8*>(glGetString(GL_VERSION)));

		Logger::Info(PString("OpenGL Info:\n\tVendor: %R\n\tRenderer: %R\n\tVersion: %R", &vendor, &renderer, &version));

#ifdef CL_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CL_CORE_ASSERT(versionMajor > 4 || versionMajor == 4 && versionMinor >= 5, "OpenGL 4.5 or newer required!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		CL_PROFILE_FUNCTION_HIGH()

		glfwSwapBuffers(WindowHandle);
	}
}

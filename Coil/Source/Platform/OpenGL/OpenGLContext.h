#pragma once

#include "Coil/Core.h"
#include "Coil/Renderer/GraphicsContext.h"


struct GLFWwindow;

namespace Coil
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* WindowHandle;
	};
}
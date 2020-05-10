#pragma once

#include "Coil/Renderer/GraphicsContext.h"


struct GLFWwindow;

namespace Coil
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext() = default;

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* WindowHandle;
	};
}
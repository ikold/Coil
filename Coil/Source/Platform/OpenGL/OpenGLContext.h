#pragma once

#include "Coil/Renderer/GraphicsContext.h"


struct GLFWwindow;


namespace Coil
{
	/**
	 * @brief 
	 *
	 *
	 * @return		
	 *
	 * @todo Write a documentation
	 */
	class OpenGLContext final : public GraphicsContext
	{
	public:
		explicit OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* WindowHandle;
	};
}

#include "pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Coil
{
	Scope<RendererAPI> RenderCommand::sRendererAPI = CreateScope<OpenGLRendererAPI>();
}

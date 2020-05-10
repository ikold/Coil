#include "pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Coil
{
	RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI();
}

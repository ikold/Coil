#include "pch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Coil
{
	RendererAPI::API RendererAPI::sAPI = API::OpenGL;

		Scope<RendererAPI> RendererAPI::Create()
		{
			switch (sAPI)
			{
			case API::None:
				CL_CORE_ASSERT(false, "RenderAPI::None unsupported!");
				return nullptr;

			case API::OpenGL:
				return CreateScope<OpenGLRendererAPI>();
			}

			CL_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
}

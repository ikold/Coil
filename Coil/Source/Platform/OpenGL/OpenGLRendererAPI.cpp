#include "pch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>


namespace Coil
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			Logger::Fatal(message);
			return;
		case GL_DEBUG_SEVERITY_MEDIUM:
			Logger::Error(message);
			return;
		case GL_DEBUG_SEVERITY_LOW:
			Logger::Warning(message);
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			// TODO suppress excessive tracing logs
			//Logger::Trace(message);
			return;
		default:
			CL_CORE_ASSERT(false, "Unknown severity level!");
		}
	}

	void OpenGLRendererAPI::Init()
	{
		CL_PROFILE_FUNCTION_HIGH()

#ifdef CL_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		CL_PROFILE_FUNCTION_HIGH()

			glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		CL_PROFILE_FUNCTION_HIGH()

			glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		CL_PROFILE_FUNCTION_HIGH()

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndex(const Ref<VertexArray>& vertexArray, uint32 indexCount)
	{
		CL_PROFILE_FUNCTION_LOW()

			const uint32 count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}

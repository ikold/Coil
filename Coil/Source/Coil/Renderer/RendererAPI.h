#pragma once

#include "VertexArray.h"


namespace Coil
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};


	public:
		RendererAPI()                       = default;
		RendererAPI(const RendererAPI& rhs) = default;
		RendererAPI(RendererAPI&& rhs)      = default;

		virtual ~RendererAPI() = default;

		RendererAPI& operator=(const RendererAPI& rhs) = default;
		RendererAPI& operator=(RendererAPI&& rhs)      = default;

		static Scope<RendererAPI> Create();

		virtual void Init() = 0;

		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndex(const Ref<VertexArray>& vertexArray) = 0;

		static API GetAPI() { return sAPI; }

	private:
		static API sAPI;
	};
}

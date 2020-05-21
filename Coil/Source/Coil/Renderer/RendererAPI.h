#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>


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

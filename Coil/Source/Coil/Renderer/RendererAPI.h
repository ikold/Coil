#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>


namespace Coil
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;


		enum class API
		{
			None = 0,
			OpenGL = 1
		};


	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndex(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		static API GetAPI() { return sAPI; }

	private:
		static API sAPI;
	};
}

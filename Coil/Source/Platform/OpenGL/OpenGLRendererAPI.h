#pragma once

#include "Coil/Renderer/RendererAPI.h"


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
	class OpenGLRendererAPI final : public RendererAPI
	{
	public:
		void Init() override;

		void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndex(const Ref<VertexArray>& vertexArray) override;
	};
}

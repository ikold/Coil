#pragma once

#include "Coil/Renderer/RendererAPI.h"


namespace Coil
{
	class OpenGLRendererAPI final : public RendererAPI
	{
	public:
		OpenGLRendererAPI()  = default;
		~OpenGLRendererAPI() = default;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndex(const Ref<VertexArray>& vertexArray) override;
	};
}

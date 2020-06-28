#pragma once

namespace Coil
{
	class GraphicsContext
	{
	public:
		GraphicsContext()                           = default;
		GraphicsContext(const GraphicsContext& rhs) = default;
		GraphicsContext(GraphicsContext&& rhs)      = default;

		virtual ~GraphicsContext() = default;

		GraphicsContext& operator=(const GraphicsContext& rhs) = default;
		GraphicsContext& operator=(GraphicsContext&& rhs)      = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}

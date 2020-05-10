#pragma once

#include "Coil/Layers/Layer.h"

namespace Coil
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		static void Begin();
		static void End();
	};
}

#pragma once

#include "Coil/Core.h"

#include "Coil/Layers/Layer.h"

namespace Coil
{
	class COIL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	};

}


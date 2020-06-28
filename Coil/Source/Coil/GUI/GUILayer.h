#pragma once

#include "Coil/Layers/Layer.h"


namespace Coil
{
	class GUILayer final : public Layer
	{
	public:
		GUILayer();
		
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void OnEvent(Event& event) override;
		
		static void Begin();
		static void End();
	};
}

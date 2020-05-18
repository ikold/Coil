#pragma once

#include "Coil/Layers/Layer.h"


namespace Coil
{
	class GUILayer : public Layer
	{
	public:
		GUILayer();
		~GUILayer() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void OnEvent(Event& event) override;
		
		static void Begin();
		static void End();
	};
}

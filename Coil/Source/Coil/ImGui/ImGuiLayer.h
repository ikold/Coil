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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		static void Begin();
		static void End();
	};

}


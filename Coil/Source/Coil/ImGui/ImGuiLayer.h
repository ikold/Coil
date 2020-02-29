#pragma once

#include "Coil/Core.h"

#include "Coil/Layers/Layer.h"

#include "Coil/Events/Event.h"
#include "Coil/Events/ApplicationEvent.h"
#include "Coil/Events/MouseEvent.h"
#include "Coil/Events/KeyEvent.h"

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

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
	};

}


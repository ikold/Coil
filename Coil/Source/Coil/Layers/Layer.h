#pragma once

#include "Coil/Events/Event.h"


namespace Coil
{
	class Layer
	{
	public:
		Layer(const RString<String>& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		[[nodiscard]] RString<String> GetName() const { return DebugName; }

	protected:
		RString<String> DebugName;
	};
}

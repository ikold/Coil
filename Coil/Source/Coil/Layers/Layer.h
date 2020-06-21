#pragma once

#include "Coil/Events/Event.h"


namespace Coil
{
	class Layer
	{
	public:
		Layer(const RString<>& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		[[nodiscard]] RString<> GetName() const { return DebugName; }

	protected:
		RString<> DebugName;
	};
}

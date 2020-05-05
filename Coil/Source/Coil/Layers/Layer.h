#pragma once

#include "Coil/Core.h"
#include "Coil/Events/Event.h"

namespace Coil
{
	class Layer
	{
	public:
		Layer(RString<String> name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline RString<String> GetName() const { return DebugName; }

	protected:
		RString<String> DebugName;
	};
}

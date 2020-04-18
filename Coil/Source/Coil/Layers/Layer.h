#pragma once

#include "Coil/Core.h"
#include "Coil/Events/Event.h"

namespace Coil
{
	class COIL_API Layer
	{
	public:
		Layer(RString name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline RString GetName() const { return DebugName; }

	protected:
		RString DebugName;
	};

}


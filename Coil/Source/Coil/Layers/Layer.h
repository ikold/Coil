#pragma once

#include "Coil/Core.h"
#include "Coil/Events/Event.h"

namespace Coil
{
	class COIL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return DebugName; }

	protected:
		std::string DebugName;
	};

}


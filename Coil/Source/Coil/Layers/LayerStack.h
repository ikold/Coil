#pragma once

#include "Coil/Core.h"
#include "Layer.h"

namespace Coil
{
	class COIL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline std::vector<Layer*>::iterator begin() { return Layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return Layers.end(); }

	private:
		std::vector<Layer*> Layers;
		uint32 LayerInsert = 0;
	};
}

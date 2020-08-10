#include "pch.h"
#include "LayerStack.h"


namespace Coil
{
	LayerStack::LayerStack() = default;

	LayerStack::~LayerStack()
	{
		for (auto* layer : Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		Layers.emplace(Layers.begin() + LayerInsertIndex, layer);
		++LayerInsertIndex;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::find(Layers.begin(), Layers.begin() + LayerInsertIndex, layer);
		if (it != Layers.begin() + LayerInsertIndex)
		{
			layer->OnDetach();
			Layers.erase(it);
			--LayerInsertIndex;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		const auto it = std::find(Layers.begin(), Layers.end(), overlay);
		if (it != Layers.end())
		{
			overlay->OnDetach();
			Layers.erase(it);
		}
	}
}

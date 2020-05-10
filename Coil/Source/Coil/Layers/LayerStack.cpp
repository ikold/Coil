#include "pch.h"
#include "LayerStack.h"


namespace Coil
{
	LayerStack::LayerStack() = default;

	LayerStack::~LayerStack()
	{
		for (auto* layer : Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		layer->OnAttach();
		Layers.emplace(Layers.begin() + LayerInsertIndex, layer);
		++LayerInsertIndex;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		overlay->OnAttach();
		Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::find(Layers.begin(), Layers.end(), layer);
		if (it != Layers.end())
		{
			Layers.erase(it);
			--LayerInsertIndex;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		const auto it = std::find(Layers.begin(), Layers.end(), overlay);
		if (it != Layers.end())
			Layers.erase(it);
	}
}

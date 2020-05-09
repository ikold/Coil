#include "pch.h"
#include "LayerStack.h"

namespace Coil
{
	LayerStack::LayerStack()
	{}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : Layers)
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
		auto it = std::find(Layers.begin(), Layers.end(), layer);
		if (it != Layers.end())
		{
			layer->OnDetach();
			Layers.erase(it);
			--LayerInsertIndex;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(Layers.begin(), Layers.end(), overlay);
		if (it != Layers.end())
		{
			overlay->OnDetach();
			Layers.erase(it);
		}
	}
}
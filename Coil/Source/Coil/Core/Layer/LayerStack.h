#pragma once

#include "Layer.h"


namespace Coil
{
	/**
	 * @brief Stack storage for Layers and Overlays
	 *
	 * @note Overlays are always at the top of the stack
	 */
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		/**
		 * @brief Adds new layer to the stack
		 *
		 * @param[in]	layer
		 *
		 * @note Layers are inserted before the overlays
		 * @note OnAttach is called on the layer after push is completed
		 * @note No checks for duplicates are made
		 */
		void PushLayer(Layer* layer);

		/**
		 * @brief Adds new overlay at the top of the stack
		 *
		 * @param[in]	overlay
		 *
		 * @note OnAttach is called on the overlay after push is completed
		 * @note No checks for duplicates are made
		 */
		void PushOverlay(Layer* overlay);


		/**
		 * @brief Removes passed layer from the stack
		 *
		 * @param[in]	layer
		 *
		 * @note OnDetach is called on the layer before popping, but only if the layer was present in the stack
		 */
		void PopLayer(Layer* layer);
		
		/**
		 * @brief Removes passed layer from the stack
		 *
		 * @param[in]	overlay
		 *
		 * @note OnDetach is called on the overlay before popping, but only if the overlay was present in the stack
		 */
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return Layers.begin(); }
		std::vector<Layer*>::iterator end() { return Layers.end(); }

	private:
		std::vector<Layer*> Layers;
		/** Helper variable to keep track of insert between layers and overlays*/
		uint32 LayerInsertIndex = 0;
	};
}

#pragma once

#include "Coil/Core/Layer/Layer.h"


namespace Coil
{
	/**
	 * @brief Layer responsible for drawing GUI 
	 */
	class GUILayer final : public Layer
	{
	public:
		GUILayer();

		void OnAttach() override;
		void OnDetach() override;
		/**
		 * @brief Draws GUI elements
		 *
		 * @todo Change ownership of GUI elements, currently static method GUI::OnImGuiRender() is called by each GUILayer
		 */
		void OnImGuiRender() override;

		/**
		 * @brief GUI event handling
		 *
		 * @param[in]	event	
		 *
		 * @todo Better integration with ImGui event handling, currently only checks if ImGui independently consumed mouse event
		 */
		void OnEvent(Event& event) override;
		
	private:
		/**
		 * @brief Starts new ImGui frame
		 */
		static void Begin();
		/**
		 * @brief Ends current ImGui frame
		 */
		static void End();
	};
}

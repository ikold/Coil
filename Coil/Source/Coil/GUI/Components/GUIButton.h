#pragma once

#include "GUIComponent.h"


namespace Coil
{
	/**
	 * @brief GUI Button with callback functionality
	 *
	 * @note Callback format: void func(void)
	 */
	class GUIButton final : public GUIComponent
	{
	public:
		GUIButton(const GUIComponentProps& properties, std::function<void()> callback);

		void Draw() const override;

		void BindCallback(const std::function<void()>& callback) { Callback = callback; }
		[[nodiscard]] const std::function<void()>& RetrieveCallback() const { return Callback; }

	private:
		std::function<void()> Callback;
	};
}

#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUIButton : public GUIComponent
	{
	public:
		GUIButton(RString<> label, void (*callback)());

		void Draw() const override;

		void BindCallback(void (*callback)()) { Callback = callback; }

		typedef void (*CallbackPtr)();
		[[nodiscard]] CallbackPtr RetrieveCallback() const { return Callback; }

	private:
		void (*Callback)();
	};
}

#pragma once

#include "Coil/Core.h"

#include "Coil/KeyCodes.h"
#include "Coil/MouseButtons.h"

namespace Coil
{
	class COIL_API Input
	{
	public:
		static inline bool IsKeyPressed(int keycode) { return Instance->IsKeyPressedImpl(keycode); }

		static inline bool IsMouseButtonPressed(int button) { return Instance->IsMouseButtonPressedImpl(button); }

		static inline std::pair<float, float> GetMousePosition() { return Instance->GetMousePositionImpl(); }

		static inline float GetMouseX() { return Instance->GetMouseXImpl(); }
		static inline float GetMouseY() { return Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;

		virtual std::pair<float, float> GetMousePositionImpl() = 0;

		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* Instance;
	};

}


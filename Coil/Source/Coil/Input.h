#pragma once

#include "Coil/Core.h"

#include "Coil/KeyCodes.h"
#include "Coil/MouseButtons.h"

namespace Coil
{
	class Input
	{
	public:
		static inline bool IsKeyPressed(int32 keycode) { return Instance->IsKeyPressedImpl(keycode); }

		static inline bool IsMouseButtonPressed(int32 button) { return Instance->IsMouseButtonPressedImpl(button); }

		static inline std::pair<float32, float32> GetMousePosition() { return Instance->GetMousePositionImpl(); }

		static inline float32 GetMouseX() { return Instance->GetMouseXImpl(); }
		static inline float32 GetMouseY() { return Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int32 keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int32 button) = 0;

		virtual std::pair<float32, float32> GetMousePositionImpl() = 0;

		virtual float32 GetMouseXImpl() = 0;
		virtual float32 GetMouseYImpl() = 0;

	private:
		static Input* Instance;
	};
}

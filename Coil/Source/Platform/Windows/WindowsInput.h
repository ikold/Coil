#pragma once

#include "Coil/Input.h"

namespace Coil
{
	class WindowsInput final : public Input
	{
	protected:
		bool IsKeyPressedImpl(int32 keycode) override;

		bool IsMouseButtonPressedImpl(int32 button) override;

		std::pair<float32, float32> GetMousePositionImpl() override;

		float32 GetMouseXImpl() override;
		float32 GetMouseYImpl() override;
	};
}

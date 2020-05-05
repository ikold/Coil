#pragma once

#include "Coil/Core.h"

#include "Coil/Input.h"

namespace Coil
{
	class COIL_API WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int32 keycode) override;

		virtual bool IsMouseButtonPressedImpl(int32 button) override;

		virtual std::pair<float32, float32> GetMousePositionImpl() override;

		virtual float32 GetMouseXImpl() override;
		virtual float32 GetMouseYImpl() override;
	};
}

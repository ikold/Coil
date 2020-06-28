#pragma once


namespace Coil
{
	class Input
	{
	public:
		Input()                 = default;
		Input(const Input& rhs) = default;
		Input(Input&& rhs)      = default;

		virtual ~Input() = default;

		Input& operator=(const Input& rhs) = default;
		Input& operator=(Input&& rhs)      = default;

		static bool IsKeyPressed(int32 keycode) { return Instance->IsKeyPressedImpl(keycode); }

		static bool IsMouseButtonPressed(int32 button) { return Instance->IsMouseButtonPressedImpl(button); }

		static std::pair<float32, float32> GetMousePosition() { return Instance->GetMousePositionImpl(); }

		static float32 GetMouseX() { return Instance->GetMouseXImpl(); }
		static float32 GetMouseY() { return Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int32 keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int32 button) = 0;

		virtual std::pair<float32, float32> GetMousePositionImpl() = 0;

		virtual float32 GetMouseXImpl() = 0;
		virtual float32 GetMouseYImpl() = 0;

	private:
		static Scope<Input> Instance;
	};
}

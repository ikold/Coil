#pragma once


namespace Coil
{
	/**
	 * @brief Base of the input utility class
	 *
	 * @note Should have derive per platform
	 * @note Static Input::Instance should be assigned in derived class
	 *
	 * @todo Add getKeyRepeatCount() method
	 */
	class Input
	{
	public:
		Input()                 = default;
		Input(const Input& rhs) = default;
		Input(Input&& rhs)      = default;

		virtual ~Input() = default;

		Input& operator=(const Input& rhs) = default;
		Input& operator=(Input&& rhs)      = default;

		/**
		 * @brief Interface for checking if selected key is pressed;
		 *
		 * @param[in]	keycode	CL_KEY_[key]
		 *
		 * @return		true if key is pressed
		 *
		 * @see KeyCodes.h for details about available keycodes
		 */
		static bool IsKeyPressed(int32 keycode) { return Instance->IsKeyPressedImpl(keycode); }

		/**
		 * @brief Interface for checking if selected key is pressed;
		 *
		 * @param[in]	button	CL_MOUSE_BUTTON_[button]
		 *
		 * @return		true if button is pressed
		 *
		 * @see MouseButtons.h for details about available button codes
		 */
		static bool IsMouseButtonPressed(int32 button) { return Instance->IsMouseButtonPressedImpl(button); }

		/**
		 * @brief Interface for getting mouse position
		 *
		 * @return		[x, y] mouse position pair counted from top-left corner
		 *
		 * @note x and y starts at (0, 0) on top-left corner
		 * @note x increments in right direction
		 * @note y increments in downward direction
		 * @note mouse position outside of the window is still returned, but might have negative values
		 *
		 * @todo Decide on the ranges for all platforms to follow
		 */
		static std::pair<float32, float32> GetMousePosition() { return Instance->GetMousePositionImpl(); }

		/**
		 * @brief Interface for getting x value of mouse position
		 *
		 * @return		x position of the mouse
		 *
		 * @see Input::GetMousePosition() for more details
		 */
		static float32 GetMouseX() { return Instance->GetMouseXImpl(); }

		/**
		 * @brief Interface for getting y value of mouse position
		 *
		 * @return		y position of the mouse
		 *
		 * @see Input::GetMousePosition() for more details
		 */
		static float32 GetMouseY() { return Instance->GetMouseYImpl(); }

	protected:
		// Implementation of the methods, to override in platform specific input class
		virtual bool IsKeyPressedImpl(int32 keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int32 button) = 0;

		virtual std::pair<float32, float32> GetMousePositionImpl() = 0;

		virtual float32 GetMouseXImpl() = 0;
		virtual float32 GetMouseYImpl() = 0;

	private:
		static Scope<Input> Instance;
	};
}

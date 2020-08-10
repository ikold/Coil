#pragma once

#include "Event.h"


namespace Coil
{
	/**
	 * @brief Mouse moved with the current mouse position
	 *
	 * @note Position is counted in pixels from the top-left corner in right-down direction
	 * @note X position is in <0, WindowWidth> range
	 * @note Y position is in <0, WindowHeight> range
	 * @note Event is created only if cursor is inside the window
	 * @note Window focus does not affect the event creation
	 * @note Event is not created if cursor moves in the boundaries of the window but it is obstructed by another window (might be platform dependent)
	 */
	class MouseMovedEvent final : public Event
	{
	public:
		MouseMovedEvent(float32 x, float32 y)
			: MouseX(x),
			  MouseY(y) {}

		[[nodiscard]] float32 GetX() const { return MouseX; }
		[[nodiscard]] float32 GetY() const { return MouseY; }

		/**
		 * @brief Converts event to printable string
		 *
		 * @return	"MouseMovedEvent: [MouseX], [MouseY]", where MouseX and MouseY are a floating numbers
		 */
		[[nodiscard]] RString<> ToString() const override { return PString("MouseMovedEvent: %f, %f", MouseX, MouseY); }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

	private:
		float32 MouseX, MouseY;
	};

	/**
	 * @brief Mouse scrolled with the current scroll offset in x and y axis
	 * 
	 * @note Event is created only if cursor is inside the window
	 * @note Window focus does not affect the event creation
	 */
	class MouseScrolledEvent final : public Event
	{
	public:
		MouseScrolledEvent(float32 x, float32 y)
			: XOffset(x),
			  YOffset(y) {}

		[[nodiscard]] float32 GetXOffset() const { return XOffset; }
		[[nodiscard]] float32 GetYOffset() const { return YOffset; }

		/**
		 * @brief Converts event to printable string
		 *
		 * @return	"MouseScrolledEvent: [XOffset], [YOffset]", where XOffset and YOffset are a floating numbers
		 */
		[[nodiscard]] RString<> ToString() const override { return PString("MouseScrolledEvent: %f, %f", XOffset, YOffset); }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

	private:
		float32 XOffset, YOffset;
	};

	/**
	 * @brief Base for the mouse button events
	 *
	 * @note Should not be used in the code
	 */
	class MouseButtonEvent : public Event
	{
	public:
		[[nodiscard]] int32 GetMouseButton() const { return Button; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::MouseButton | EventCategory::Input)

	protected:
		explicit MouseButtonEvent(int32 button)
			: Button(button) {}

		int32 Button;
	};

	/**
	 * @brief Mouse button pressed event with the button code
	 *
	 * @note Event is created only if window is focused
	 * @note If window losses focus due to mouse button press, event is not created for it
	 * @note Event is not repeated for held button
	 */
	class MouseButtonPressedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent(int32 button)
			: MouseButtonEvent(button) {}
		
		/**
		 * @brief Converts event to printable string
		 *
		 * @return	"MouseButtonPressedEvent: [Button]", where Button is a button code
		 */
		[[nodiscard]] RString<> ToString() const override { return PString("MouseButtonPressedEvent: %5d", Button); }

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	/**
	 * @brief Mouse button released event with the button code
	 */
	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent(int32 button)
			: MouseButtonEvent(button) {}

		/**
		 * @brief Converts event to printable string
		 *
		 * @return	"MouseButtonReleasedEvent: [Button]", where Button is a button code
		 */
		[[nodiscard]] RString<> ToString() const override { return PString("MouseButtonReleasedEvent: %5d", Button); }

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

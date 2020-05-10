#pragma once

#include "Event.h"


namespace Coil
{
	class MouseMovedEvent final : public Event
	{
	public:
		MouseMovedEvent(float32 x, float32 y)
			: MouseX(x),
			  MouseY(y)
		{}

		[[nodiscard]] float32 GetX() const { return MouseX; }
		[[nodiscard]] float32 GetY() const { return MouseY; }

		[[nodiscard]] RString<String> ToString() const override { return PString("MouseMovedEvent: %f, %f", MouseX, MouseY); }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

	private:
		float32 MouseX, MouseY;
	};


	class MouseScrolledEvent final : public Event
	{
	public:
		MouseScrolledEvent(float32 x, float32 y)
			: XOffset(x),
			  YOffset(y)
		{}

		[[nodiscard]] float32 GetXOffset() const { return XOffset; }
		[[nodiscard]] float32 GetYOffset() const { return YOffset; }

		[[nodiscard]] RString<String> ToString() const override { return PString("MouseScrolledEvent: %f, %f", XOffset, YOffset); }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

	private:
		float32 XOffset, YOffset;
	};


	class MouseButtonEvent : public Event
	{
	public:
		[[nodiscard]] int32 GetMouseButton() const { return Button; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::MouseButton | EventCategory::Input)

	protected:
		MouseButtonEvent(int32 button)
			: Button(button)
		{}

		int32 Button;
	};


	class MouseButtonPressedEvent final : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int32 button)
			: MouseButtonEvent(button)
		{}

		[[nodiscard]] RString<String> ToString() const override { return PString("MouseButtonPressedEvent: %5d", Button); }

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};


	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int32 button)
			: MouseButtonEvent(button)
		{}

		[[nodiscard]] RString<String> ToString() const override { return PString("MouseButtonReleasedEvent: %5d", Button); }

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

#pragma once

#include "Event.h"

namespace Coil
{
	class COIL_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float32 x, float32 y)
			: MouseX(x), MouseY(y)
		{}

		inline float32 GetX() const { return MouseX; }
		inline float32 GetY() const { return MouseY; }

		RString ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << MouseX << ", " << MouseY;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	private:
		float32 MouseX, MouseY;
	};

	class COIL_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float32 x, float32 y)
			: XOffset(x), YOffset(y)
		{}

		inline float32 GetXOffset() const { return XOffset; }
		inline float32 GetYOffset() const { return YOffset; }

		RString ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << XOffset << ", " << YOffset;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	private:
		float32 XOffset, YOffset;
	};

	class COIL_API MouseButtonEvent : public Event
	{
	public:
		inline int32 GetMouseButton() const { return Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput);

	protected:
		MouseButtonEvent(int32 button)
			: Button(button)
		{}

		int32 Button;
	};

	class COIL_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int32 button)
			: MouseButtonEvent(button)
		{}

		RString ToString() const override
		{
			return SString() << "MouseButtonPressedEvent: " << Button;
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class COIL_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int32 button)
			: MouseButtonEvent(button)
		{}

		RString ToString() const override
		{
			return SString() << "MouseButtonReleasedEvent: " << Button;
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}
#pragma once

#include "Event.h"

namespace Coil
{
	class COIL_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: MouseX(x), MouseY(y) {}

		inline float GetX() const { return MouseX; }
		inline float GetY() const { return MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << MouseX << ", " << MouseY;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float MouseX, MouseY;
	};

	class COIL_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y)
			: XOffset(x), YOffset(y) {}

		inline float GetXOffset() const { return XOffset; }
		inline float GetYOffset() const { return YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << XOffset << ", " << YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float XOffset, YOffset;
	};

	class COIL_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)

	protected:
		MouseButtonEvent(int button)
			: Button(button) {}

		int Button;
	};

	class COIL_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class COIL_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
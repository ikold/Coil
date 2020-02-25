#pragma once

#include "Event.h"

namespace Coil
{
	class COIL_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: Width(width), Height(height) {}

		inline unsigned int GetWidth() const { return Width; }
		inline unsigned int GetHeight() const { return Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << Width << ", " << Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory::Application)

	private:
		unsigned int Width, Height;
	};

	class COIL_API WindowCloseEvent : public Event
	{
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class COIL_API WindowFocusEvent : public Event
	{
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class COIL_API WindowLostFocusEvent : public Event
	{
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class COIL_API WindowMovedEvent : public Event
	{
		WindowMovedEvent(int x, int y)
			: PositionX(x), PositionY(y) {}

		inline int GetPositionX() const { return PositionX; }
		inline int GetPositionY() const { return PositionY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << PositionX << ", " << PositionY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategory::Application)

	private:
		int PositionX, PositionY;
	};

	class COIL_API AppTickEvent : public Event
	{
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class COIL_API AppUpdateEvent : public Event
	{
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class COIL_API AppRenderEvent : public Event
	{
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};
}
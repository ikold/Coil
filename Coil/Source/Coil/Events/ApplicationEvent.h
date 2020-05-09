#pragma once

#include "Event.h"

namespace Coil
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height)
			: Width(width), Height(height)
		{}

		uint32 GetWidth() const { return Width; }
		uint32 GetHeight() const { return Height; }

		RString<String> ToString() const override
		{
			return PString("WindowResizeEvent: %6d, %6d", Width, Height);
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategory::Application);

	private:
		uint32 Width, Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategory::Application);
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus);
		EVENT_CLASS_CATEGORY(EventCategory::Application);
	};

	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus);
		EVENT_CLASS_CATEGORY(EventCategory::Application);
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int32 x, int32 y)
			: PositionX(x), PositionY(y)
		{}

		int32 GetPositionX() const { return PositionX; }
		int32 GetPositionY() const { return PositionY; }

		RString<String> ToString() const override
		{
			return PString("WindowMovedEvent: %6d, %6d", PositionX, PositionY);
		}

		EVENT_CLASS_TYPE(WindowMoved);
		EVENT_CLASS_CATEGORY(EventCategory::Application);

	private:
		int32 PositionX, PositionY;
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategory::Application);
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategory::Application);
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategory::Application);
	};
}
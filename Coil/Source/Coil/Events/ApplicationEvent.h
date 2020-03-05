#pragma once

#include "Event.h"

namespace Coil
{
	class COIL_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height)
			: Width(width), Height(height) {}

		inline uint32 GetWidth() const { return Width; }
		inline uint32 GetHeight() const { return Height; }

		RString ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << Width << ", " << Height;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		uint32 Width, Height;
	};

	class COIL_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class COIL_API WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class COIL_API WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class COIL_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int32 x, int32 y)
			: PositionX(x), PositionY(y) {}

		inline int32 GetPositionX() const { return PositionX; }
		inline int32 GetPositionY() const { return PositionY; }

		RString ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << PositionX << ", " << PositionY;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		int32 PositionX, PositionY;
	};

	class COIL_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class COIL_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class COIL_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
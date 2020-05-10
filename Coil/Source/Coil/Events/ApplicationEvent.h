#pragma once

#include "Event.h"

namespace Coil
{
	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height)
			: Width(width), Height(height)
		{}

		[[nodiscard]] uint32 GetWidth() const { return Width; }
		[[nodiscard]] uint32 GetHeight() const { return Height; }

		[[nodiscard]] RString<String> ToString() const override
		{
			return PString("WindowResizeEvent: %6d, %6d", Width, Height);
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory::Application)

	private:
		uint32 Width, Height;
	};

	class WindowCloseEvent final : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class WindowFocusEvent final : public Event
	{
	public:
		WindowFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class WindowLostFocusEvent final : public Event
	{
	public:
		// TO-DO
		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class WindowMovedEvent final : public Event
	{
	public:
		WindowMovedEvent(int32 x, int32 y)
			: PositionX(x), PositionY(y)
		{}

		[[nodiscard]] int32 GetPositionX() const { return PositionX; }
		[[nodiscard]] int32 GetPositionY() const { return PositionY; }

		[[nodiscard]] RString<String> ToString() const override
		{
			return PString("WindowMovedEvent: %6d, %6d", PositionX, PositionY);
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategory::Application)

	private:
		int32 PositionX, PositionY;
	};

	class AppTickEvent final : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class AppUpdateEvent final : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	class AppRenderEvent final : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};
}
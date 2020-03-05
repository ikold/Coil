#pragma once

#include "Coil/Core.h"

#include <string>
#include <sstream>
#include <functional>

namespace Coil
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		EventCategoryNone =			0,
		EventCategoryApplication =	1 << 0,
		EventCategoryInput =			1 << 1,
		EventCategoryKeyboard =		1 << 2,
		EventCategoryMouse =			1 << 3,
		EventCategoryMouseButton =	1 << 4
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class COIL_API Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual RString ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlags() & (int)category;
		}

		inline bool IsHandled() { return Handled; }
	protected:
		bool Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: EventToDispatch(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (EventToDispatch.GetType() == T::GetStaticType())
			{
				EventToDispatch.Handled = func(*(T*)&EventToDispatch);
				return true;
			}
			return false;
		}

	private:
		Event& EventToDispatch;
	};

#define BIND_EVENT_FN(fn) std::bind(&fn, std::placeholders::_1)
#define BIND_EVENT_METHOD(fn) std::bind(&fn, this, std::placeholders::_1)
}
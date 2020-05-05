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

	namespace EventCategory
	{
		enum Enum
		{
			None = 0,
			Application = 1 << 0,
			Input = 1 << 1,
			Keyboard = 1 << 2,
			Mouse = 1 << 3,
			MouseButton = 1 << 4
		};
	}

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetType() const override { return GetStaticType(); }\
								virtual RString<String> GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int32 GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetType() const = 0;
		virtual RString<String> GetName() const = 0;
		virtual int32 GetCategoryFlags() const = 0;
		virtual RString<String> ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory::Enum category) const
		{
			return GetCategoryFlags() & category;
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
			: EventToDispatch(event)
		{}

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
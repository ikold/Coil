#pragma once

#include "Event.h"

namespace Coil
{
	class COIL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keyCode)
			: KeyCode(keyCode) {}

		int KeyCode;
	};

	class COIL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return RepeatCount; }

		RString ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << KeyCode << " (" << RepeatCount << " repeats)";
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int RepeatCount;
	};

	class COIL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		RString ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << KeyCode;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class COIL_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode)
		{
		}

		RString ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << KeyCode;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
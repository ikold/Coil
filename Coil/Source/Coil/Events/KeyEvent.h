#pragma once

#include "Event.h"

namespace Coil
{
	class COIL_API KeyEvent : public Event
	{
	public:
		inline int32 GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int32 keyCode)
			: KeyCode(keyCode) {}

		int32 KeyCode;
	};

	class COIL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int32 keyCode, int32 repeatCount)
			: KeyEvent(keyCode), RepeatCount(repeatCount) {}

		inline int32 GetRepeatCount() const { return RepeatCount; }

		RString ToString() const override
		{
			return SString() << "KeyPressedEvent: " << KeyCode << " (" << RepeatCount << " repeats)";
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int32 RepeatCount;
	};

	class COIL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32 keyCode)
			: KeyEvent(keyCode) {}

		RString ToString() const override
		{
			return SString() << "KeyReleasedEvent: " << KeyCode;
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class COIL_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int32 keyCode)
			: KeyEvent(keyCode)
		{
		}

		RString ToString() const override
		{
			return SString() << "KeyTypedEvent: " << KeyCode;
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
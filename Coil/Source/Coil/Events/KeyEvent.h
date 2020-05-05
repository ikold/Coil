#pragma once

#include "Event.h"

namespace Coil
{
	class KeyEvent : public Event
	{
	public:
		inline int32 GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input);

	protected:
		KeyEvent(int32 keyCode)
			: KeyCode(keyCode)
		{}

		int32 KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int32 keyCode, int32 repeatCount)
			: KeyEvent(keyCode), RepeatCount(repeatCount)
		{}

		inline int32 GetRepeatCount() const { return RepeatCount; }

		RString<String> ToString() const override
		{
			return PString("KeyPressedEvent: %4d (%4d repeats)", KeyCode, RepeatCount);
		}

		EVENT_CLASS_TYPE(KeyPressed);

	private:
		int32 RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32 keyCode)
			: KeyEvent(keyCode)
		{}

		RString<String> ToString() const override
		{
			return PString("KeyReleasedEvent: %4d", KeyCode);
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int32 keyCode)
			: KeyEvent(keyCode)
		{}

		RString<String> ToString() const override
		{
			return PString("KeyTypedEvent: %4d", KeyCode);
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};
}
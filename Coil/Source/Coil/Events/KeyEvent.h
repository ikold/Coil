#pragma once

#include "Event.h"


namespace Coil
{
	class KeyEvent : public Event
	{
	public:
		[[nodiscard]] int32 GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)

	protected:
		KeyEvent(int32 keyCode)
			: KeyCode(keyCode)
		{}

		int32 KeyCode;
	};


	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(int32 keyCode, int32 repeatCount)
			: KeyEvent(keyCode),
			  RepeatCount(repeatCount)
		{}

		[[nodiscard]] int32 GetRepeatCount() const { return RepeatCount; }

		[[nodiscard]] RString<String> ToString() const override { return PString("KeyPressedEvent: %4d (%4d repeats)", KeyCode, RepeatCount); }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int32 RepeatCount;
	};


	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32 keyCode)
			: KeyEvent(keyCode)
		{}

		[[nodiscard]] RString<String> ToString() const override { return PString("KeyReleasedEvent: %4d", KeyCode); }

		EVENT_CLASS_TYPE(KeyReleased)
	};


	class KeyTypedEvent final : public KeyEvent
	{
	public:
		KeyTypedEvent(int32 keyCode)
			: KeyEvent(keyCode)
		{}

		[[nodiscard]] RString<String> ToString() const override { return PString("KeyTypedEvent: %4d", KeyCode); }

		EVENT_CLASS_TYPE(KeyTyped)
	};
}

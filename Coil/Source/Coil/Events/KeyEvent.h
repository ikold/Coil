#pragma once

#include "Event.h"


namespace Coil
{
	/**
	 * @brief Base for the key events
	 *
	 * @note Should not be used in the code
	 */
	class KeyEvent : public Event
	{
	public:
		[[nodiscard]] int32 GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)

	protected:
		explicit KeyEvent(int32 keyCode)
			: KeyCode(keyCode) {}

		int32 KeyCode;
	};


	/**
	 * @brief Key pressed event with the key code and the repeat count
	 *
	 * @note Key code corresponds to Coil key codes
	 * @note First event for the given key has repeat count equal 0
	 */
	class KeyPressedEvent final : public KeyEvent
	{
	public:
		KeyPressedEvent(int32 keyCode, int32 repeatCount)
			: KeyEvent(keyCode),
			  RepeatCount(repeatCount) {}

		[[nodiscard]] int32 GetRepeatCount() const { return RepeatCount; }

		/**
		 * @brief Converts event to printable string
		 *
		 * @return	"KeyPressedEvent: [KeyCode] ([RepeatCount] repeats)", where KeyCode and RepeatCount are a signed integers
		 */
		[[nodiscard]] RString<> ToString() const override { return PString("KeyPressedEvent: %4d (%4d repeats)", KeyCode, RepeatCount); }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int32 RepeatCount;
	};

	/**
	 * @brief Key release event with the key code
	 *
	 * @note Key code corresponds to Coil key codes
	 */
	class KeyReleasedEvent final : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(int32 keyCode)
			: KeyEvent(keyCode) {}

		/**
		 * @brief Converts event to printable string
		 *
		 * @return	"KeyReleasedEvent: [KeyCode]", where KeyCode is a signed integers
		 */
		[[nodiscard]] RString<> ToString() const override { return PString("KeyReleasedEvent: %4d", KeyCode); }

		EVENT_CLASS_TYPE(KeyReleased)
	};

	/**
	 * @brief Key typed event with the ASCII value
	 *
	 * @note Always corresponds to the lower case letters and digits on the number row.
	 * @note Non ASCII keys do not create this event (e.g. Ctrl, Shift)
	 * @note If the key is held, new event is created for each repeat
	 *
	 * @todo Create additional event that takes into the consideration capitalization of the letters
	 */
	class KeyTypedEvent final : public KeyEvent
	{
	public:
		explicit KeyTypedEvent(int32 keyCode)
			: KeyEvent(keyCode) {}
		
		/**
		 * @brief Converts event to printable string
		 *
		 * @return	"KeyTypedEvent: [KeyCode]", where KeyCode is a signed integers in ASCII representation
		 */
		[[nodiscard]] RString<> ToString() const override { return PString("KeyTypedEvent: %4d", KeyCode); }

		EVENT_CLASS_TYPE(KeyTyped)
	};
}

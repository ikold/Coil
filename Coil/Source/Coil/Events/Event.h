#pragma once


namespace Coil
{
	enum class EventType
	{
		None = 0,

		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,

		AppTick,
		AppUpdate,
		AppRender,

		KeyPressed,
		KeyReleased,
		KeyTyped,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
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
								virtual RString<> GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int32 GetCategoryFlags() const override { return category; }


	/**
	 * @brief Base event class
	 */
	class Event
	{
		friend class EventDispatcher;

	public:
		Event()                 = default;
		Event(const Event& rhs) = default;
		Event(Event&& rhs)      = default;

		virtual ~Event() = default;

		Event& operator=(const Event& rhs) = default;
		Event& operator=(Event&& rhs)      = default;

		[[nodiscard]] virtual EventType GetType() const = 0;
		[[nodiscard]] virtual RString<> GetName() const = 0;
		[[nodiscard]] virtual int32 GetCategoryFlags() const = 0;
		[[nodiscard]] virtual RString<> ToString() const { return GetName(); }

		[[nodiscard]] bool IsInCategory(EventCategory::Enum category) const { return GetCategoryFlags() & category; }

		[[nodiscard]] bool IsHandled() const { return Handled; }

	protected:
		/** Controls propagation of the event */
		bool Handled = false;
	};


	/**
	 * @brief Dispatches event to the function
	 */
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		/**
		 * @brief Creates dispatcher for given event
		 *
		 * @param[in]	event	Event to dispatch
		 */
		explicit EventDispatcher(Event& event)
			: EventToDispatch(event) {}

		/**
		 * @brief Dispatches event to the function
		 *
		 * @tparam[in]	TEvent	Event class that should be dispatched
		 * @param[in]	func	Function that returns bool and takes TEvent reference as the only parameter (e.g. bool function(TEvent& event))
		 *
		 * @return		true if event were dispatch
		 *
		 * @note Event is dispatched only if it type is the same as the TEvent
		 * @note If event was dispatch, Handled flag is set with the returned value of the function
		 * @note Dispatcher does not check if event is already handled
		 */
		template<typename TEvent>
		bool Dispatch(EventFn<TEvent> func)
		{
			if (EventToDispatch.GetType() == TEvent::GetStaticType())
			{
				EventToDispatch.Handled = func(*static_cast<TEvent*>(&EventToDispatch));
				return true;
			}
			return false;
		}

	private:
		Event& EventToDispatch;
	};
}


/**
 * @brief Bind macro for the EventDispatcher
 *
 * @param[in]	function	Function to bind that returns bool and takes TEvent reference as the only parameter (e.g. bool function(TEvent& event))
 *
 * @note Usage example DispatcherObject.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressedFunction));
 */
#define BIND_EVENT_FN(function) std::bind(&function, std::placeholders::_1)

/**
 * @brief Bind macro for the EventDispatcher
 *
 * @param[in]	method	Method to bind that returns bool and takes TEvent reference as the only parameter (e.g. bool Class::Method(TEvent& event))
 *
 * @note Usage example DispatcherObject.Dispatch<KeyPressedEvent>(BIND_EVENT_METHOD(Class::OnKeyPressedF));
 */
#define BIND_EVENT_METHOD(method) std::bind(&method, this, std::placeholders::_1)

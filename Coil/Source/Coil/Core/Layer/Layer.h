#pragma once


namespace Coil
{
	/**
	 * @brief Base class for game layers
	 */
	class Layer
	{
	public:
		/**
		 * @brief Default constructor
		 *
		 * @param[in]	name	Debug name
		 */
		explicit Layer(RString<> name = "Layer");

		/** Copy constructor */
		Layer(const Layer& rhs) = default;
		/** Move constructor */
		Layer(Layer&& rhs)      = default;

		virtual ~Layer() = default;

		/** Copy assignment operator */
		Layer& operator=(const Layer& rhs) = default;
		/** Move assignment operator */
		Layer& operator=(Layer&& rhs)      = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		[[nodiscard]] RString<> GetName() const { return DebugName; }

	protected:
		RString<> DebugName;
	};
}

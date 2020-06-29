#pragma once


namespace Coil
{
	class Layer
	{
	public:
		explicit Layer(RString<> name = "Layer");

		Layer(const Layer& rhs) = default;
		Layer(Layer&& rhs)      = default;

		virtual ~Layer() = default;

		Layer& operator=(const Layer& rhs) = default;
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

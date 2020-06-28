#pragma once


namespace Coil
{
	class Layer
	{
	public:
		explicit Layer(RString<> name = "Layer");
		virtual ~Layer() = default;

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

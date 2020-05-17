#pragma once


namespace Coil
{
	struct GUIComponentProps
	{
		RString<String> Label;
		float32 Width;
		float32 Height;
		bool SameLine;

		GUIComponentProps(const RString<>& label = "", float32 width = 0, float32 height = 0, bool sameLine = false)
			: Label(PString("%s##%d", label->CString(), LabelID++)),
			  Width(width),
			  Height(height),
			  SameLine(sameLine) {}

		static int32 LabelID;
	};


	class GUIComponent
	{
	public:
		GUIComponent(const GUIComponentProps& properties)
			: Properties(properties) {}

		virtual ~GUIComponent() = default;

		virtual void SetUp() const;

		virtual void Draw() const = 0;

	protected:
		GUIComponentProps Properties;
	};
}

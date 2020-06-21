#pragma once


namespace Coil
{
	struct GUIComponentProps
	{
		RString<> Label = "";
		float32 Width   = 0;
		float32 Height  = 0;
		bool SameLine   = false;


		inline static int32 DafaultLabelID = 0;
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

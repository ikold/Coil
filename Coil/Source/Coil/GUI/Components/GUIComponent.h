#pragma once


namespace Coil
{
	struct GUIComponentProps
	{
		RString<> Label = "";
		float32 Width   = 0;
		float32 Height  = 0;
		bool SameLine   = false;

		int32 ID = ++DefaultLabelID;

		inline static int32 DefaultLabelID = 0;
	};


	class GUIComponent
	{
	public:
		explicit GUIComponent(GUIComponentProps properties)
			: Properties(Move(properties))
		{}

		GUIComponent(const GUIComponent& rhs) = default;
		GUIComponent(GUIComponent&& rhs)      = default;

		virtual ~GUIComponent() = default;

		GUIComponent& operator=(const GUIComponent& rhs) = default;
		GUIComponent& operator=(GUIComponent&& rhs)      = default;

		GUIComponentProps& GetProperties() { return Properties; }

		virtual void SetUp() const;

		virtual void Draw() const = 0;

	protected:
		GUIComponentProps Properties;
	};
}

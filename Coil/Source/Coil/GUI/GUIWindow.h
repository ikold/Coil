#pragma once


namespace Coil
{
	static int DefaultNameId = 0;


	struct GUIWindowProps
	{
		RString<String> Name;
		int32 Width;
		int32 Height;

		GUIWindowProps(const RString<String>& name = PString("Window %d", DefaultNameId++).ToString(), int32 width = 512, int32 height = 256)
			: Name(name),
			Width(width),
			Height(height)
		{}
	};
	
	class GUIWindow
	{
	public:
		GUIWindow(const GUIWindowProps& properties);

		virtual ~GUIWindow() = default;

		void OnImGuiRender() const;

	protected:
		[[nodiscard]] int32 TextTopCulling() const;
		void TextBottomCulling(int32 linesTotal) const;
		[[nodiscard]] int32 WindowSizeInLines() const;

		virtual void SetUp() const = 0;
		virtual void Draw() const = 0;

	protected:
		GUIWindowProps Properties;
		
		bool* OpenFlag;
		int Flags;

	private:
		float32 FontHeight         = 17.f;
		float32 DummyDefaultOffset = 4.f;
	};
}

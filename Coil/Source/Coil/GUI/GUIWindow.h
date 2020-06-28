#pragma once


namespace Coil
{
	struct GUIWindowProps
	{
		RString<> Name = "";
		int32 Width    = 512;
		int32 Height   = 256;
		bool Visible   = true;
	};


	class GUIWindow
	{
	public:
		explicit GUIWindow(GUIWindowProps properties);

		GUIWindow(const GUIWindow& rhs) = default;
		GUIWindow(GUIWindow&& rhs)      = default;

		virtual ~GUIWindow() = default;

		GUIWindow& operator=(const GUIWindow& rhs) = default;
		GUIWindow& operator=(GUIWindow&& rhs)      = default;

		void OnImGuiRender() const;

		[[nodiscard]] GUIWindowProps& GetProperties() { return Properties; }

	protected:
		[[nodiscard]] int32 TextTopCulling() const;
		void TextBottomCulling(int32 linesTotal) const;
		[[nodiscard]] int32 WindowSizeInLines() const;

		virtual void SetUp() const {}
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

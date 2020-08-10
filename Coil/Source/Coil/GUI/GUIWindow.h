#pragma once


namespace Coil
{
	/**
	 * @brief Properties for GUI window
	 *
	 * @param	Name		
	 * @param	Width		
	 * @param	Height		
	 * @param	Visible		Determinates if window should be drawn
	 *
	 * @todo System for handling same names for different windows
	 */
	struct GUIWindowProps
	{
		RString<> Name = "";
		int32 Width    = 512;
		int32 Height   = 256;
		bool Visible   = true;
	};


	/**
	 * @brief Base class for GUI Windows
	 */
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
		/**
		 * @brief Draws empty element for occluded lines above displayed text
		 *
		 * @return	Number of culled lines
		 */
		[[nodiscard]] int32 TextTopCulling() const;
		/**
		 * @brief Draws empty element for occluded lines below displayed text
		 */
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

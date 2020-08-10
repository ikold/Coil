#pragma once


namespace Coil
{
	/**
	 * @brief Properties for GUI component
	 *
	 * @param	Label		
	 * @param	Width		
	 * @param	Height		
	 * @param	SameLine	Draw component in the same line as the preceding component
	 * @param	ID			ID of the component
	 *
	 * @note ID should not be changed and use default value
	 */
	struct GUIComponentProps
	{
		RString<> Label = "";
		float32 Width   = 0;
		float32 Height  = 0;
		bool SameLine   = false;

		int32 ID = ++DefaultLabelID;

		inline static int32 DefaultLabelID = 0;
	};

	/**
	 * @brief Base class for the components
	 */
	class GUIComponent
	{
	public:
		explicit GUIComponent(GUIComponentProps properties);

		GUIComponent(const GUIComponent& rhs) = default;
		GUIComponent(GUIComponent&& rhs)      = default;

		virtual ~GUIComponent() = default;

		GUIComponent& operator=(const GUIComponent& rhs) = default;
		GUIComponent& operator=(GUIComponent&& rhs)      = default;

		GUIComponentProps& GetProperties() { return Properties; }

		
		/**
		 * @brief Preparers component for drawing
		 *
		 * @note After calling this method only the caller should be draw
		 */
		virtual void SetUp() const;
		/**
		 * @brief Resets GUI settings from the SetUp()
		 */
		virtual void CleanUp() const;

		/**
		 * @brief Draws GUI on the screen
		 *
		 * @note Should be called between SetUp() and CleanUp()
		 */
		virtual void Draw() const = 0;

	protected:
		GUIComponentProps Properties;
	};
}

#pragma once


namespace Coil
{
	class ImGuiWindow
	{
	public:
		ImGuiWindow(const RString<String>& name, int32 width, int32 height);

		virtual ~ImGuiWindow() = default;

		void OnImGuiRender() const;

	protected:
		[[nodiscard]] int32 TextTopCulling() const;
		void TextBottomCulling(int32 linesTotal) const;
		[[nodiscard]] int32 WindowSizeInLines() const;

		virtual void Draw() const = 0;

	protected:
		bool* OpenFlag;
		int Flags;

	private:
		RString<String> Name;
		int32 Width;
		int32 Height;

		float32 FontHeight         = 17.f;
		float32 DummyDefaultOffset = 4.f;
	};
}

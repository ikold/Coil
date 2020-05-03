#pragma once

#include "pch.h"

#include "Coil/Core.h"
#include "Coil/Events/Event.h"
#include "Coil/Events/ApplicationEvent.h"
#include "Coil/Events/MouseEvent.h"
#include "Coil/Events/KeyEvent.h"

namespace Coil
{
	struct WindowProps
	{
		RString<String> Name;
		uint32 Width;
		uint32 Height;

		WindowProps(RString<String> name = "Coil Engine", uint32 width = 1280, uint32 height = 720)
			:Name(name), Width(width), Height(height)
		{}
	};

	class COIL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
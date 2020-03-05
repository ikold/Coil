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
		RString Name;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const RString& name = "Coil Engine", unsigned int width = 1280, unsigned int height = 720)
			:Name(name), Width(width), Height(height) {}
	};

	class COIL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
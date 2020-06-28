#include "pch.h"
#include "WindowsInput.h"

#include "Coil/Application.h"
#include <GLFW/glfw3.h>


namespace Coil
{
	Scope<Input> Input::Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int32 keycode)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		const auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int32 button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		const int32 state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float32, float32> WindowsInput::GetMousePositionImpl()
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		float64 xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { static_cast<float32>(xpos), static_cast<float32>(ypos) };
	}

	float32 WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float32 WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}

#include "pch.h"
#include "GUILayer.h"

#include "Coil/Core/Application.h"

#include "imgui.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

#include <GLFW/glfw3.h>

#include "GUI.h"


namespace Coil
{
	GUILayer::GUILayer()
		: Layer("GUILayer")
	{}

	void GUILayer::OnAttach()
	{
		CL_PROFILE_FUNCTION_HIGH()

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		//Loads font with "zero-width space" in place of "return" character
		io.Fonts->AddFontFromFileTTF("Resources/Fonts/ProggyClean_ZeroWidthReturn.ttf", 13.0f);
		io.Fonts->AddFontFromFileTTF("Resources/Fonts/ProggyClean_ZeroWidthReturn_Debug.ttf", 13.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding              = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		auto* window     = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void GUILayer::OnDetach()
	{
		CL_PROFILE_FUNCTION_HIGH()

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUILayer::OnImGuiRender()
	{
		Begin();

		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		GUI::OnImGuiRender();

		End();
	}

	void GUILayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>([]([[maybe_unused]] MouseScrolledEvent& event) -> bool
		{
			ImGuiIO& io = ImGui::GetIO();
			return io.WantCaptureMouse;
		});

		dispatcher.Dispatch<MouseButtonPressedEvent>([]([[maybe_unused]] MouseButtonPressedEvent& event) -> bool
		{
			ImGuiIO& io = ImGui::GetIO();
			return io.WantCaptureMouse;
		});
	}

	void GUILayer::Begin()
	{
		CL_PROFILE_FUNCTION_HIGH()

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GUILayer::End()
	{
		CL_PROFILE_FUNCTION_HIGH()

		ImGuiIO& io      = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2(static_cast<float32>(app.GetWindow().GetWidth()), static_cast<float32>(app.GetWindow().GetHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_Current_Context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_Current_Context);
		}
	}
}

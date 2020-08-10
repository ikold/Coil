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
		: Layer("GUILayer") {}

	void GUILayer::OnAttach()
	{
		CL_PROFILE_FUNCTION_HIGH()

		IMGUI_CHECKVERSION();

		{
			CL_PROFILE_SCOPE_MEDIUM("Creating ImGui Context")

			ImGui::CreateContext();
		}

		ImGuiIO& io = ImGui::GetIO();

		{
			CL_PROFILE_SCOPE_MEDIUM("Loading Fonts")

			// Loads font with "zero-width space" in place of "return" character
			io.Fonts->AddFontFromFileTTF("Resources/Fonts/ProggyClean_ZeroWidthReturn.ttf", 13.0f);
			io.Fonts->AddFontFromFileTTF("Resources/Fonts/ProggyClean_ZeroWidthReturn_Debug.ttf", 13.0f);
		}

		{
			CL_PROFILE_SCOPE_MEDIUM("Setting ImGui Flags")

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui::StyleColorsDark();

			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding              = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}
		}

		{
			CL_PROFILE_SCOPE_MEDIUM("Initializating ImGui for OpengGL")

			Application& app = Application::Get();
			auto* window     = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}
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
		CL_PROFILE_FUNCTION_HIGH()

		Begin();

		// Makes whole application window transparent dock for ImGui windows to latch on
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		// Draws GUI windows in static class GUI
		GUI::OnImGuiRender();

		End();
	}

	void GUILayer::OnEvent(Event& event)
	{
		CL_PROFILE_FUNCTION_HIGH()

		EventDispatcher dispatcher(event);

		// If ImGui wants to handle these events, marking them as dispatched to prevent further propagation
		
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

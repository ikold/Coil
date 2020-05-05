project "GLFW"
	location "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"GLFW/include/GLFW/glfw3.h",
		"GLFW/include/GLFW/glfw3native.h",
		"GLFW/src/glfw_config.h",
		"GLFW/src/context.c",
		"GLFW/src/init.c",
		"GLFW/src/input.c",
		"GLFW/src/monitor.c",
		"GLFW/src/vulkan.c",
		"GLFW/src/window.c"
	}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"GLFW/src/win32_init.c",
			"GLFW/src/win32_joystick.c",
			"GLFW/src/win32_monitor.c",
			"GLFW/src/win32_time.c",
			"GLFW/src/win32_thread.c",
			"GLFW/src/win32_window.c",
			"GLFW/src/wgl_context.c",
			"GLFW/src/egl_context.c",
			"GLFW/src/osmesa_context.c"
		}

		defines 
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		runtime "Release"
		optimize "On"


project "GLAD"
	location "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"GLAD/include/glad/glad.h",
		"GLAD/include/KHR/khrplatform.h",
		"GLAD/src/glad.c",
	}

	includedirs
	{
		"GLAD/include"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		runtime "Release"
		optimize "On"


project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"ImGui/imconfig.h",
		"ImGui/imgui.h",
		"ImGui/imgui.cpp",
		"ImGui/imgui_draw.cpp",
		"ImGui/imgui_internal.h",
		"ImGui/imgui_widgets.cpp",
		"ImGui/imgui_rectpack.h",
		"ImGui/imgui_textedit.h",
		"ImGui/imgui_truetype.h",
		"ImGui/imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		runtime "Release"
		optimize "On"
		
		
project "Ryu"
	location "Ryu"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"Ryu/ryu/ryu.h",
		"Ryu/ryu/d2fixed.c",
		"Ryu/ryu/d2s.c",
		"Ryu/ryu/f2s.c",
	}
	
	includedirs
	{
		"Ryu"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		runtime "Release"
		optimize "On"
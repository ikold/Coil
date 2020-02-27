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

	filter "system:linux"
		systemversion "latest"
		pic "On"

		files
		{
			"GLFW/src/x11_init.c",
			"GLFW/src/x11_monitor.c",
			"GLFW/src/x11_window.c",
			"GLFW/src/xkb_unicode.c",
			"GLFW/src/posix_time.c",
			"GLFW/src/posix_thread.c",
			"GLFW/src/glx_context.c",
			"GLFW/src/egl_context.c",
			"GLFW/src/osmesa_context.c",
			"GLFW/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
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

	filter "system:linux"
		systemversion "latest"
		pic "On"

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
workspace "Coil"
	architecture "x64"
	startproject "Application"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"] = "Coil/Vendor/GLFW/include"
	IncludeDir["GLAD"] = "Coil/Vendor/GLAD/include"
	IncludeDir["ImGui"] = "Coil/Vendor/ImGui"
	IncludeDir["Ryu"] = "Coil/Vendor/Ryu"
	IncludeDir["glm"] = "Coil/Vendor/glm"
	
	group "Dependencies"
		include "Coil/Vendor"
	group ""

	project "Coil"
		location "Coil"
		kind "SharedLib"
		language "C++"
		staticruntime "Off"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
		pchheader "pch.h"
		pchsource "%{prj.name}/Source/pch.cpp"
		
		files
		{
			"%{prj.name}/Source/**.h",
			"%{prj.name}/Source/**.cpp"
		}
		
		includedirs 
		{
			"%{prj.name}/Source",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.Ryu}",
			"%{IncludeDir.glm}"
		}

		links
		{
			"GLFW",
			"GLAD",
			"ImGui",
			"Ryu",
			"opengl32.lib"
		}

		defines
		{
			"CL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"

			postbuildcommands
			{
				{"{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Application/\""},
				{"{COPY} ../Resources \"../bin/" .. outputdir .. "/Application/Resources\""},
				{"{COPY} ../Resources \"../Application/Resources\""}
			}
			
			defines
			{
				"CL_PLATFORM_WINDOWS",
				"_CRT_SECURE_NO_WARNINGS"
			}

		filter "system:linux"
			cppdialect "C++17"
			systemversion "latest"
			
			defines
			{
				"CL_PLATFORM_LINUX"
			}
			
		filter "configurations:Debug"
			runtime "Debug"
			symbols "On"
			
			defines
			{
				"CL_DEBUG",
				"CL_ENABLE_ASSERTS"
			}
			
		filter "configurations:Release"
			runtime "Release"
			optimize "On"

			defines
			{
				"CL_RELEASE"
			}
			
		filter "configurations:Distribution"
			runtime "Release"
			optimize "On"
			
			defines
			{
				"CL_DIST"
			}
			
			
	project "Application"
		location "Application"
		kind "ConsoleApp"
		language "C++"
		staticruntime "Off"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
		files
		{
			"%{prj.name}/Source/**.h",
			"%{prj.name}/Source/**.cpp"
		}
		
		includedirs
		{
			"Coil/Source"
		}
		
		links
		{
			"Coil"
		}
		
		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"
			
			defines
			{
				"CL_PLATFORM_WINDOWS"
			}

		filter "system:linux"
			cppdialect "C++17"
			systemversion "latest"
			
			defines
			{
				"CL_PLATFORM_LINUX"
			}

			debugenvs
			{
				"LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(WorkspacePath)/bin/" .. outputdir .. "/Coil/"
			}

		filter "configurations:Debug"
			runtime "Debug"
			symbols "On"
			
			defines
			{
				"CL_DEBUG",
				"CL_ENABLE_ASSERTS"
			}
			
		filter "configurations:Release"
			runtime "Release"
			optimize "On"
			
			defines
			{
				"CL_RELEASE"
			}
			
		filter "configurations:Distribution"
			runtime "Release"
			optimize "On"
			
			defines
			{
				"CL_DIST"
			}
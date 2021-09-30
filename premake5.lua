workspace "Coil"
	architecture "x86_64"
	startproject "Application"

	configurations
	{
		"Debug",
		"Release",
		"Release-Profiler",
		"Distribution"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"] = "Coil/Vendor/GLFW/include"
	IncludeDir["GLAD"] = "Coil/Vendor/GLAD/include"
	IncludeDir["ImGui"] = "Coil/Vendor/ImGui"
	IncludeDir["Ryu"] = "Coil/Vendor/Ryu"
	IncludeDir["glm"] = "Coil/Vendor/glm"
	IncludeDir["stb_image"] = "Coil/Vendor/stb_image"

	group "Dependencies"
		include "Coil/Vendor"
	group ""

	project "Coil"
		location "Coil"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "On"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
		pchheader "pch.h"
		pchsource "%{prj.name}/Source/pch.cpp"
		
		files
		{
			"%{prj.name}/Source/**.h",
			"%{prj.name}/Source/**.cpp",
			"%{prj.name}/Vendor/stb_image/**.h",
			"%{prj.name}/Vendor/stb_image/**.cpp"
		}
		
		includedirs 
		{
			"%{prj.name}/Source",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.Ryu}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stb_image}"
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
			"GLFW_INCLUDE_NONE"
		}
		
		filter "system:windows"
			systemversion "latest"
			
		filter "configurations:Debug"
			runtime "Debug"
			symbols "On"
			
			defines
			{
				"CL_DEBUG",
				"CL_PROFILE=3",
				"CL_ENABLE_ASSERTS"
			}
			
		filter "configurations:Release"
			runtime "Release"
			optimize "On"

			defines
			{
				"CL_RELEASE"
			}
			
		filter "configurations:Release-Profiler"
			runtime "Release"
			optimize "On"
			
			defines
			{
				"CL_RELEASE",
				"CL_PROFILE=3"
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
		cppdialect "C++17"
		staticruntime "On"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")
		
		files
		{
			"%{prj.name}/Source/**.h",
			"%{prj.name}/Source/**.cpp"
		}
		
		includedirs
		{
			"Coil/Source",
			"%{IncludeDir.glm}"
		}
		
		links
		{
			"Coil"
		}
		
		postbuildcommands
		{
			{"{COPY} ../Resources \"../bin/" .. outputdir .. "/Application/Resources\""},
			{"{COPY} ../Resources \"../Application/Resources\""}
		}
		
		filter "system:windows"
			systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "On"
			
			defines
			{
				"CL_DEBUG",
				"CL_PROFILE=3",
				"CL_ENABLE_ASSERTS"
			}
			
		filter "configurations:Release"
			runtime "Release"
			optimize "On"
			
			defines
			{
				"CL_RELEASE"
			}
			
		filter "configurations:Release-Profiler"
			runtime "Release"
			optimize "On"
			
			defines
			{
				"CL_RELEASE",
				"CL_PROFILE=3"
			}
			
		filter "configurations:Distribution"
			runtime "Release"
			optimize "On"
			
			defines
			{
				"CL_DIST"
			}
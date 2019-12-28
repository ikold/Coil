workspace "Coil"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}
	
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	
	project "Coil"
		location "Coil"
		kind "SharedLib"
		language "C++"
		
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
			"%{prj.name}/Source"
		}
		
		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"
			staticruntime "Off"
			
			defines
			{
				"CL_PLATFORM_WINDOWS",
				"CL_BUILD_DLL",
				"_CRT_SECURE_NO_WARNINGS"
			}
			
		filter "configurations:Debug"
			defines { "CL_DEBUG", "CL_ENABLE_ASSERTS" }
			symbols "On"
			
		filter "configurations:Release"
			defines { "CL_RELEASE" }
			optimize "On"
			
		filter "configurations:Distribution"
			defines { "CL_DIST" }
			optimize "On"
			
			
	project "Application"
		location "Application"
		kind "ConsoleApp"
		language "C++"
		
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
			staticruntime "Off"
			
			defines
			{
				"CL_PLATFORM_WINDOWS"
			}
			
			postbuildcommands
			{
				{"{COPY} ../bin/" .. outputdir .. "/Coil/Coil.dll ../bin/" .. outputdir .. "/Application"}
			}
			
		filter "configurations:Debug"
			defines { "CL_DEBUG" }
			symbols "On"
			
		filter "configurations:Release"
			defines { "CL_RELEASE" }
			optimize "On"
			
		filter "configurations:Distribution"
			defines { "CL_DIST" }
			optimize "On"
			
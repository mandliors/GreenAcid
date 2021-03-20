outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "GreenAcid"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}

	project "GreenAcid"
		location "GreenAcid"
		kind "ConsoleApp"
		language "C++"

		targetdir ("bin/" .. outputdir)
		objdir ("obj/" .. outputdir)

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.c",
			"%{prj.name}/src/**.cpp",
			"Oxygen/src/**.h",
			"Oxygen/src/**.c",
			"Oxygen/src/**.cpp"
		}

		includedirs
		{
			"Dependencies/include",
			"Oxygen/src",
			"%{wks.name}/src"
		}

		libdirs
		{
			"Dependencies/lib"
		}

		links
		{
			"glfw3",
			"opengl32"
		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "Off"
			systemversion "latest"

			defines
			{
				"_CONSOLE"
			}

		filter "configurations:Debug"
			defines "_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "NDEBUG"
			optimize "On"
			
	--------------------------------------------
	
	project "Oxygen"
		location "Oxygen"
		kind "StaticLib"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.c",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"Dependencies/include",
			"%{prj.name}/src"
		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "Off"
			systemversion "latest"

		filter "configurations:Debug"
			defines "OX_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "OX_RELEASE"
			optimize "On"
			
			outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
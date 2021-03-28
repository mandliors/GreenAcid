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
        cppdialect "C++17"

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

		filter "system:windows"
			includedirs
			{
				"WindowsDependencies/include",
				"Oxygen/src",
				"%{wks.name}/src"
			}
			libdirs
			{
				"WindowsDependencies/lib"
			}
			defines
			{
				"WINDOWS"
			}
		
		filter "system:linux"
			includedirs
			{
				"LinuxDependencies/include",
				"Oxygen/src",
				"%{wks.name}/src"
			}
			defines
			{
				"LINUX"
			}

		filter "system:windows"
			systemversion "latest"
			links
			{
				"glfw3",
				"opengl32"
			}

		filter "system:linux"
			systemversion "latest"	
			links
			{
				"glfw3",
				"GL",
				"GLU",
				"dl",
				"X11",
				"Xxf86vm",
				"pthread",
				"Xi"
			}
			--buildoptions  --FOR DEBUGGING PURPOSES
			--{
			--	"-O0",
			--	"-g3",
			--	"-ggdb3",
			--	"-fno-inline",
            --	"-Wall"
			--}

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
        cppdialect "C++17"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("obj/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.c",
			"%{prj.name}/src/**.cpp"
		}

		filter "system:windows"
			systemversion "latest"
			includedirs
			{
				"WindowsDependencies/include",
				"%{prj.name}/src"
			}
			defines
			{
				"WINDOWS"
			}
			
		filter "system:linux"
			systemversion "latest"
			includedirs
			{
				"LinuxDependencies/include",
				"%{prj.name}/src"
			}
			defines
			{
				"LINUX"
			}
			--buildoptions  --FOR DEBUGGING PURPOSES
			--{
			--	"-O0",
			--	"-g3",
			--	"-ggdb3",
			--	"-fno-inline",
            --	"-Wall"
			--}
		
		filter "configurations:Debug"
			defines "OX_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "OX_RELEASE"
			optimize "On"
			
			outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "SR"
	language "C++"
	cppdialect "C++20"
	architecture "x86_64"
	startproject "SoftwareRasterizer"
	configurations { "Debug", "Release" }

	filter { "configurations:Debug" }
		symbols "On"

	filter { "configurations:Release" }
		optimize "On"

	filter { }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SoftwareRasterizer"
	location "SoftwareRasterizer"
    kind "WindowedApp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

	includedirs
	{
		"SoftwareRasterizer/include"
	}

	
	filter "system:windows"
        systemversion "latest"
        links
        {
            "user32",
            "gdi32"
        }
	
	filter "configurations:Debug"
		defines "SR_DEBUG"
		runtime "Debug"
		symbols "on"

    filter "configurations:Release"
		defines "SR_RELEASE"
		runtime "Release"
		optimize "on"
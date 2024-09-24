project "Simple_PCI8501"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++11"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		includedirs
		{
			"%{IncludeDir.ART}"
		}

		libdirs
		{
			"%{LinkDir.ART8501}"
		}

		links
		{
			"PCI8501_64.lib"
		}
    files
    {
        "**.h",
        "**.cpp",
        "**.rc",
    }
    
    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        staticruntime "off"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        staticruntime "on"

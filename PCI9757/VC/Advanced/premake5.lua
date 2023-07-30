project "Sys"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "StdAfx.h"
    pchsource "%{wks.location}/NMROSCore/vendor/ART/ARTSamples/PCI9757/VC/Advanced/StdAfx.cpp"
    
    characterset ("ASCII")
    
    defines
    {
      "_AFXDLL"
    }

    flags
    {
      "MFC"
    }

    includedirs
    {
        "%{IncludeDir.ART}",
    }
    libdirs
    {
        "%{wks.location}/NMROSCore/vendor/ART/libs/PCI9757",
    }
    links
    {
        "PCI9757_64.lib",
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

project "Sys_8916"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "StdAfx.h"
    pchsource "%{wks.location}/NMROSCore/vendor/ART/ARTSamples/ACTS1000/VC/Advanced/StdAfx.cpp"
    
    characterset ("Unicode")
    
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
        "%{wks.location}/NMROSCore/vendor/ART/libs/ACTS1000",
    }
    links
    {
        "ACTS1000_64.lib",
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

project "HDPS_2C"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++11"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "StdAfx.h"
    pchsource "%{wks.location}/NMROSCore/vendor/Camera/HDPS/StdAfx.cpp"
    
    characterset ("ASCII")
    
    defines
    {
      "_AFXDLL"
    }

    flags
    {
      "MFC"
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

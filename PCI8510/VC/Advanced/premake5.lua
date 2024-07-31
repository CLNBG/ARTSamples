project "Sys_8510"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "StdAfx.h"
    pchsource "%{wks.location}/NMROSCore/vendor/ART/ARTSamples/PCI8510/VC/Advanced/StdAfx.cpp"
    
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
        "%{wks.location}/NMROSCore/vendor/ART/libs/PCI8510",
    }
    links
    {
        "PCI8510_64.lib",
    }
		debugenvs
  	{
    		"path=" .. os.getenv("path") .. ";" .. "%{dllDir.PCI8510}"
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

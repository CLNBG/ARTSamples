project "Sys_8916"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "StdAfx.h"
    pchsource "%{wks.location}/NMROSCore/vendor/ART/ARTSamples/PCI8501/VC/Advanced/StdAfx.cpp"
    
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
        "%{wks.location}/NMROSCore/vendor/ART/libs/PCI8501",
    }
    links
    {
        "PCI8501_64.lib",
    }
		debugenvs
  	{
    		"path=" .. os.getenv("path") .. ";" .. "%{dllDir.PCI8501}"
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

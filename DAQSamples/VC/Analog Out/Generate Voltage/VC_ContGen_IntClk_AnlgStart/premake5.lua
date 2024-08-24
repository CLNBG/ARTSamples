project "VC_ContGen_IntClk_AnlgStart"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"


	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
      "VC_ContGen_IntClk_AnlgStart.cpp",
  }
    
  includedirs
  {
      "%{IncludeDir.glm}",
      "%{IncludeDir.EIGEN}",
      "%{IncludeDir.MagicEnum}",
      "%{IncludeDir.ART}",
      "%{wks.location}/NMROSCore/vendor/DAQSys/ArtDAQ/src",
      "%{wks.location}/NMROSCore/src",
      "%{wks.location}/NMROSCore/vendor/spdlog/include",
  }

  defines
  {
    	"NMROS_ENABLE_ASSERT",
  }

  libdirs
  {
			"%{LinkDir.ARTDAQ64}"
  }
    
  links
  {
			"NMROSCore",
			"%{libsName.ARTDAQ}"
  }

  filter "system:windows"
      systemversion "latest"

  filter "configurations:Debug"
      defines "NMROS_DEBUG"
      runtime "Debug"
      symbols "on"
      staticruntime "off"

  filter "configurations:Release"
      defines "NMROS_RELEASE"
      runtime "Release"
      optimize "on"
      staticruntime "on"

  filter "configurations:Dist"
      defines "NMROS_DIST"
      runtime "Release"
      optimize "on"
      staticruntime "on"

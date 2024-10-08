project "VC_Acq_IntClk"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"


	targetdir (ARTSamplesBaseDir .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (ARTSamplesBaseDir .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
      "VC_Acq_IntClk.cpp",
  }
    
  includedirs
  {
      "%{IncludeDir.glm}",
      "%{IncludeDir.EIGEN}",
      "%{IncludeDir.MagicEnum}",
      "%{IncludeDir.ART}",
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
			"Art_DAQ.lib",
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

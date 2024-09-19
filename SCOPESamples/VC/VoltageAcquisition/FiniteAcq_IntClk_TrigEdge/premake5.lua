project "FiniteAcq_IntClk_TrigEdge"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"


	targetdir (ARTSamplesBaseDir .. "/bin/" .. outputdir .. "/%{prj.name}")
	objdir (ARTSamplesBaseDir .. "/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
      "**.cpp",
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
			"%{LinkDir.ARTSCOPE64}"
  }
    
  links
  {
			"NMROSCore",
			"Art_SCOPE.lib",
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

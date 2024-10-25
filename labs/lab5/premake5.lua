project "lab5"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"

   targetdir ("%{wks.location}/Bin/bin/" .. outputdir .. "/%{prj.name}") -- bin/Debug-Windows-x64/
   objdir ("%{wks.location}/Bin/bin-int/" .. outputdir .. "/%{prj.name}") -- bin-int/Debug-Windows-x64/

   files 
   { 
      "src/**.h",
      "src/**.hpp",
      "src/**.cpp",
   }

   includedirs
   {
      "%{IncludeDir.opencv1}",
      "%{IncludeDir.opencv2}",
      "%{IncludeDir.eigen3}",
   }
   
   libdirs
   {
      "%{LibDir.opencv}"
   } 

   filter "system:windows"      -- staticruntime "On"
        systemversion "latest" -- Windows SDK version

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      staticruntime "off"
      runtime "Debug"
      links
      {
          "opencv_world4100d"
      }
      postbuildcommands
      {
          "{COPY} %{wks.location}/3dparty/opencv/build/x64/vc16/bin/opencv_world4100d.dll $(targetdir)"
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      staticruntime "off"
      runtime "Release"
      links
      {
        "opencv_world4100.lib"
      }
      postbuildcommands
      {
          "{COPY} %{wks.location}/3dparty/opencv/build/x64/vc16/bin/opencv_world4100.dll $(targetdir)"
      }
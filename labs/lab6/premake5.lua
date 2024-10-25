project "lab6"
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

   }
   
   libdirs
   {

   } 

   filter "system:windows"      -- staticruntime "On"
        systemversion "latest" -- Windows SDK version

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      staticruntime "off"
      runtime "Debug"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      staticruntime "off"
      runtime "Release"
workspace "GAMES101"
   architecture "x64"
   root_dir = path.getabsolute(".")
   configurations 
   { 
      "Debug", 
      "Release",
   }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" -- Debug-Windows-x64

-- Include directories relative to root folder (solution directory)

IncludeDir = {}
IncludeDir["opencv1"] = "%{wks.location}/3dparty/opencv/build/include"
IncludeDir["opencv2"] = "%{wks.location}/3dparty/opencv/sources/include"
IncludeDir["eigen3"] = "%{wks.location}/3dparty/eigen3"

LibDir = {}
LibDir["opencv"] = "%{wks.location}/3dparty/opencv/build/x64/vc16/lib"

include "labs/lab1"
include "labs/lab2"
include "labs/lab3"
include "labs/lab5"
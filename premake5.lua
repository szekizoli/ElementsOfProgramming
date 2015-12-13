solution "ElementsOfProgramming-Exercises"
    configurations  { "Debug", "Release" }
    startproject    "GraphLib-unittest"

    location "build"

project "eop"
    kind      "ConsoleApp"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "src/**.h", 
            "src/**.cpp" }

    includedirs { "src" }
    filter "configurations:Debug"
    	defines {"DEBUG"}
    	flags {"Symbols"}
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
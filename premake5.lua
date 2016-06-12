solution "ElementsOfProgramming-Exercises"
    configurations  { "Debug", "Release" }
    startproject    "eop-unittest"

    location "build"

project "eop"
    kind      "ConsoleApp"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "src/eop/**.h", 
            "src/eop/**.cpp" }

    includedirs { "src" }
    filter "configurations:Debug"
    	defines {"DEBUG"}
    	flags {"Symbols"}
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

project "epwc"
    kind      "ConsoleApp"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "src/epwc/**.h", 
            "src/epwc/**.cpp" }

    includedirs { "src" }
    filter "configurations:Debug"
        defines {"DEBUG"}
        flags {"Symbols"}
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

project "GoogleTest"
    kind      "StaticLib"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "../3rd/gtest-1.7.0/src/gtest_main.cc", 
            "../3rd/gtest-1.7.0/src/gtest-all.cc" }

    libdirs { "../3rd/" }
    includedirs {"../3rd/gtest-1.7.0", "../3rd/gtest-1.7.0/include"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        flags {"Symbols"}
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

project "eop-unittest"
    kind      "ConsoleApp"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "src/unittest/**.h", 
            "src/unittest/**.cpp" }

    links {"eop", "epwc", "GoogleTest"}
    libdirs { "../3rd/" }
    includedirs {"../3rd/gtest-1.7.0", "../3rd/gtest-1.7.0/include", "src/eop", "src/epwc" }
    filter "configurations:Debug"
        defines {"DEBUG"}
        flags {"Symbols"}
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
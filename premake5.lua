
solution "ElementsOfProgramming-Exercises"
    configurations  { "Debug", "Release" }
    startproject    "eop-unittest"

    location "build"

project "eop"
    kind      "StaticLib"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "src/eop/**.h", 
            "src/eop/**.cpp" }

    includedirs { "src" }
    filter "configurations:Debug"
    	defines {"DEBUG"}
    	symbols "On"
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
    
    configuration "gmake"
        buildoptions {
            "-std=c++14"
        }

project "epwc"
    kind      "StaticLib"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "src/epwc/**.h",
            "src/epwc/**.cpp" }

    includedirs { "src" }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

    configuration "gmake"
        buildoptions {
            "-std=c++14"
        }

project "GoogleTest"
    kind      "StaticLib"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "../3rd/gtest-1.8.0/googletest/src/gtest_main.cc", 
            "../3rd/gtest-1.8.0/googletest/src/gtest-all.cc" }

    libdirs { "../3rd/" }
    includedirs {"../3rd/gtest-1.8.0/googletest", "../3rd/gtest-1.8.0/googletest/include"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
 
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

    configuration "gmake"
        buildoptions {
            "-std=c++14"
        }

project "eop-test"
    kind      "ConsoleApp"
    language  "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "test/**.h", 
            "test/**.cpp" }

    links {"eop", "epwc", "GoogleTest"}
    libdirs { "../3rd/" }
    includedirs {"../3rd/gtest-1.8.0/googletest", "../3rd/gtest-1.8.0/googletest/include", "src/eop", "src/epwc" }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

    configuration "gmake"
        buildoptions {
            "-std=c++14"
        }
        linkoptions {
            "-pthread"
        }


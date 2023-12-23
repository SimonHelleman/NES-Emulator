-- premake5.lua
workspace "NES-Emulator"
    configurations { "Debug", "Release" }

project "NES-Emulator"
    kind "ConsoleApp"
    language "C++"
    architecture "x64"
    targetdir "bin/%{cfg.buildcfg}"

    files { "**.h", "**.c", "**.cpp" }
    includedirs { "Dependencies/GLAD/include", "Dependencies/imgui/include" }
    
    filter { "system:windows" }
		includedirs { "Dependencies/GLFW/include" }
	
	filter { "system:windows", "action:vs*" }
        libdirs { "Dependencies/GLFW/win64/lib-vc2019" }
        links { "glfw3", "opengl32" }
    
    filter { "system:windows", "action:gmake2" }
        libdirs { "Dependencies/GLFW/win64/lib-mingw-w64" }
        links { "glfw3", "gdi32", "opengl32" }
		
	filter { "system:linux" }
		links { "glfw", "GL" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

workspace "Helios"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    warnings = { "4267", "4244",  "4302", "4311", "4312", "4067", "26495", "26451", "26812" }   -- i hate warnings with a passion

    project "HeliosEventSystem"
        kind            "StaticLib"
        language        "C++"
        cppdialect      "C++17"
        staticruntime   "On"
        location        "Source"
                        
        targetdir       ("bin/"     .. outputdir .. "/%{prj.name}")
        objdir          ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        disablewarnings { warnings }
    
        files
        {
            "Source/**.hpp",
            "Source/**.cpp",
        }
    
        includedirs
        {
            "Source",
        }
    
        filter "system:windows"
            systemversion "latest"
    
        filter "configurations:Debug"
            defines "DEBUG"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "On"
    
    project "Examples"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "On"
        location "Examples"
    
        targetdir ("bin/"     .. outputdir .. "/%{prj.name}")
        objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        disablewarnings { warnings }
    
        files
        {
            "Examples/**.hpp",
            "Examples/**.cpp",
        }
    
        includedirs
        {
            "Source",
        }
    
        links
        {
            "HeliosEventSystem"
        }
    
        filter "system:windows"
            systemversion "latest"
    
        filter "configurations:Debug"
            defines "DEBUG"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "On"
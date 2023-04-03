function SetBXCompat()
	filter "action:vs*"
		includedirs{"%{prj.location}/common/include/compat/msvc"}
	filter {"system:windows", "action:gmake"}
		includedirs{"%{prj.location}/common/include/compat/mingw"}
	filter {"system:macosx"}
		includedirs {"%{prj.location}/common/include/compat/osx"}
		buildoptions {"-x objective-c++"}
end


----------------------------------------------
-- Mint library
----------------------------------------------
	project "Mint"
		print("\n")
		print("Defining Project \"Mint\"")
		location "Mint/Mint"
		kind "StaticLib"
		language "C++"
		architecture "x86_64"
		cppdialect "C++17"
		-- use latest available system version (e.g. latest Windows SDK)
		systemversion "latest"
		staticruntime "Off"

		filter "action:vs*"
			buildoptions{
				"/Zc:__cplusplus"
			}

		-- Note: In order to create nested vpaths the "deepest" ones have to be defined first,
		-- e.g. first define "Renderer/BGFX" and after it "Renderer".
		-- This is a premake5 specific issue.
		mint_location = "Mint/Mint"
		mint_dirs_to_load = {
			"Common",
			"Utility/EventSystem",
			"Utility/FileSystem",
			"Utility/Logging",
			"Utility/MAML/Common",
			"Utility/MAML",
			"Utility/PluginSystem",
			"Utility/Profiling",
			"Utility/Serialization",
			"Utility/ServiceSystem",
			"Utility/STL",
			"Utility",
			"Components",
			"SceneSystem/Common",
			"SceneSystem",
			"Graphics/Common/EmbeddedShaders/Shaders/android",
			"Graphics/Common/EmbeddedShaders/Shaders/ios",
			"Graphics/Common/EmbeddedShaders/Shaders/linux",
			"Graphics/Common/EmbeddedShaders/Shaders/orbis",
			"Graphics/Common/EmbeddedShaders/Shaders/osx",
			"Graphics/Common/EmbeddedShaders/Shaders/windows",
			"Graphics/Common/EmbeddedShaders/Shaders",
			"Graphics/Common/EmbeddedShaders",
			"Graphics/Common",
			"Graphics/SUSManager",
			"Graphics",
			"RessourceManagement/Common/RessourceLoaders",
			"RessourceManagement/Common",
			"RessourceManagement",
			"Physics/Common",
			"Physics",
			"UI/Common/imgui",
			"UI/Common",
			"UI",
		}


		-- Specify which source files to include
		mint_abs_path = path.getabsolute(mint_location)
		mint_abs_path = mint_abs_path .. "/src"
		print("Loading files from " .. mint_abs_path)
		-- Load project interface files
		files{
				mint_location .. "/Mint.h", 
				mint_location .. "/Mint.cpp",
				mint_location .. "/IMint.h", 
				mint_location .. "/IMint.cpp",
			}
		-- Allow including from src
		includedirs{
			mint_abs_path
		}

		-- Load project folders and create virtual paths
		for index = 1, #mint_dirs_to_load do
			fldr = mint_dirs_to_load[index]
			print("Loading folder: " .. "[" .. fldr.."]" .. " = " .. mint_abs_path .. "/" .. fldr)
			-- Create virtual path in solution
			vpaths{
				[fldr] = {mint_abs_path .. "/" .. fldr .. "/**.h", mint_abs_path .. "/" .. fldr .. "/**.cpp"}
			}
			-- Load files to project
			files{
				mint_abs_path .. "/" .. fldr .. "/**.h", mint_abs_path .. "/" .. fldr .. "/**.cpp"
			}
			-- Allow including folder headers across project
			print("Create Include folder: " .. fldr)
			includedirs{
				fldr
			}
		end

		-- Includes
		includedirs{
			"%{prj.location}/common/include",
			"%{prj.location}/common/include/spdlog",
		}


		-- Platform specific defines and linking
		group "Linking"
			include "mint_linker.lua"
		group ""

		SetBXCompat()

		group "Defines"
			include "mint_defines.lua"
		group ""


		print("\nDo not forget following steps for a clean build:\n")
		print("1.) Set working directory correctly for debugging in your IDE!\n")
		print("2.) Disable usage of vcpkg if using Visual Studio!\n")
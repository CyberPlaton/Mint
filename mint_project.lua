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
		floatingpoint "Fast"

		filter "action:vs*"
			buildoptions{
				"/Zc:__cplusplus"
			}


		-- Note: In order to create nested vpaths the "deepest" ones have to be defined first,
		-- e.g. first define "Renderer/BGFX" and after it "Renderer".
		-- This is a premake5 specific issue.
		mint_location = "Mint/Mint"
		mint_dirs_to_load = {
			"Common/LuaBinding",
			"Common",
			"Utility/EventSystem",
			"Utility/FileSystem",
			"Utility/Logging",
			"Utility/MAML/Common",
			"Utility/MAML",
			"Utility/PluginSystem",
			"Utility/Profiling",
			"Utility/Serialization",
			"Utility/ServiceSystem/Services",
			"Utility/ServiceSystem",
			"Utility/STL",
			"Utility/Input/LuaBinding",
			"Utility/Input",
			"Utility/Reflection",
			"Utility",
			"Components/LuaBinding",
			"Components",
			"SceneSystem/LuaBinding",
			"SceneSystem/Common",
			"SceneSystem",
			"Graphics/LuaBinding",
			"Graphics/Common",
			"Graphics/PrimitiveRenderer",
			"Graphics/DebugRenderer",
			"Graphics/SceneRenderer",
			"Graphics/Shaders/EmbeddedShaders",
			"Graphics/Shaders/ShaderGraph",
			"Graphics/Shaders/Materials",
			"Graphics/Shaders/ShaderConstructor/Common/ShaderPreprocessor",
			"Graphics/Shaders/ShaderConstructor/Common",
			"Graphics/Shaders/ShaderConstructor",
			"Graphics/Shaders",
			"Graphics",
			"RessourceManagement/Common/RessourceLoaders",
			"RessourceManagement/Common",
			"RessourceManagement",
			"Physics/Common/box2d",
			"Physics/Common",
			"Physics/LuaBinding",
			"Physics",
			"UI/Common/imgui",
			"UI/Common",
			"UI",
			"Scripting/LuaBinding",
			"Scripting/Behavior",
			"Scripting/Script",
			"Scripting/Common",
			"Scripting",
			"Hardware/hwinfo/utils",
			"Hardware/hwinfo/apple/utils",
			"Hardware/hwinfo/linux/utils",
			"Hardware/hwinfo/windows/utils",
			"Hardware/hwinfo",
			"Hardware/apple/utils",
			"Hardware/windows/utils",
			"Hardware/linux/utils",
			"Hardware/apple",
			"Hardware/windows",
			"Hardware/linux",
			"Hardware",
			
			"Editor/EditorLayer/Common",
			"Editor/EditorLayer/Layers",
			"Editor/EditorLayer",
			"Editor/RessourceManagement",
			"Editor/Common",
			"Editor/TextEditor",
			"Editor/ComponentEditor/Common",
			"Editor/ComponentEditor/ComponentEditors",
			"Editor/ComponentEditor",
			"Editor",
		}


		-- Specify which source files to include
		mint_abs_path = path.getabsolute(mint_location)
		mint_abs_path = mint_abs_path .. "/src"
		print("Loading files from " .. mint_abs_path)
		-- Load project interface files
		files{
				mint_location .. "/Application.h", 
				mint_location .. "/Application.cpp",
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

			
			print("Create Virtual folder: [" .. fldr .. "] = " .. mint_abs_path)
			vpaths{
				[fldr] = {mint_abs_path .. "/" .. fldr .. "/**.h", mint_abs_path .. "/" .. fldr .. "/**.cpp"}
			}
			-- Load files to project
			files{
				mint_abs_path .. "/" .. fldr .. "/**.h", mint_abs_path .. "/" .. fldr .. "/**.cpp",
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
			"%{prj.location}/common/include/raylib",
			"%{prj.location}/src/Physics/Common",
		}
		print("Include third party files...")


		-- Platform specific defines and linking
		group "Linking"
			include "mint_linker.lua"
		group ""

		group "Defines"
			include "mint_defines.lua"
		group ""


		print("\nDo not forget following steps for a clean build:\n")
		print("1.) Set working directory correctly for debugging in your IDE!\n")
		print("2.) Disable usage of vcpkg if using Visual Studio!\n")
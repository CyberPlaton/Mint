----------------------------------------------
-- Mint Sandbox application
----------------------------------------------
	project "Sandbox"
		print("\n")
		print("Defining Project \"Sandbox\"")
		location "Sandbox/Sandbox"
		kind "ConsoleApp"
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


		mint_engine_path = path.getabsolute("Mint")
		includedirs{
			mint_engine_path .. "/Mint",
			mint_engine_path .. "/Mint/src"
		}
		print("Include Mint engine header and source files at: " .. mint_engine_path .. "/Mint, " .. mint_engine_path .. "/Mint/src")


		scenes_and_plugins_path = os.getcwd() .. "/Sandbox/Sandbox/src"
		scenes_and_plugins = {
			"BuiltinPlugins",
			"CustomPlugins",
			"Scenes",
		}
		print("Include scene and plugins sources and create virtual paths for them at: " .. scenes_and_plugins_path)


		print("Loading scenes and plugins:")
		for index = 1, #scenes_and_plugins do
			fldr = scenes_and_plugins[index]
			print("[" .. fldr.."]" .. " = " .. scenes_and_plugins_path .. "/" .. fldr)
			vpaths{
				[fldr] = {scenes_and_plugins_path .. "/" .. fldr .. "/**.h", scenes_and_plugins_path .. "/" .. fldr .. "/**.cpp"}
			}
			files{
				scenes_and_plugins_path .. "/" .. fldr .. "/**.h", scenes_and_plugins_path .. "/" .. fldr .. "/**.cpp"
			}
			includedirs{
				fldr
			}
		end


		files{
			"%{prj.name}/%{prj.name}/Main.h",
			"%{prj.name}/%{prj.name}/Main.cpp",
		}
		includedirs{
			"%{prj.name}/%{prj.name}/src"
		}
		print("Include project source and header files...")


		includedirs{
			"%{prj.location}/common/include",
			"%{prj.location}/common/include/spdlog",
			"%{prj.location}/common/include/raylib",
		}
		print("Include third party files...")


		print("Linking of third party libraries and Mint engine...")
		group "Linking"
			include "mint_sandbox_linker.lua"
			include "mint_sandbox_mint_engine_linker.lua"
		group ""

		print("Create common defines...")
		group "Defines"
			include "mint_sandbox_defines.lua"
		group ""

		print("Completed!")
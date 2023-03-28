----------------------------------------------
-- Mint Editor application
----------------------------------------------
	project "Editor"
		print("\n")
		print("Defining Project \"Editor\"")
		location "Editor/Editor"
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


		source_path = os.getcwd() .. "/Sandbox/Sandbox/src"
		source_folders = {
			"Common"
		}
		for index = 1, #source_folders do
			fldr = source_folders[index]
			print("[" .. fldr.."]" .. " = " .. source_path .. "/" .. fldr)
			vpaths{
				[fldr] = {source_path .. "/" .. fldr .. "/**.h", source_path .. "/" .. fldr .. "/**.cpp"}
			}
			files{
				source_path .. "/" .. fldr .. "/**.h", source_path .. "/" .. fldr .. "/**.cpp"
			}
			print("Create Include folder: " .. fldr)
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
		}
		print("Include third party files...")


		print("Linking of third party libraries and Mint engine...")
		group "Linking"
			include "mint_editor_linker.lua"
			include "mint_editor_mint_engine_linker.lua"
		group ""

		SetBXCompat()

		print("Create common defines...")
		group "Defines"
			include "mint_editor_defines.lua"
		group ""

		print("Completed!")
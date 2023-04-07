		mint_location = "Mint/Mint"
		mint_shader_dir_location = "Graphics/Common/EmbeddedShaders/Shaders"
		mint_shader_dirs = {
			"android",
			"ios",
			"linux",
			"orbis",
			"osx",
			"windows",
		}
		mint_shaders_to_load = {
			"sprite",
			"backbuffer"
		}


		-- Specify which source files to include
		mint_abs_path = path.getabsolute(mint_location)
		mint_abs_path = mint_abs_path .. "/src"
		print("Loading shaders from " .. mint_abs_path .. "/" .. mint_shader_dir_location)
		-- Load project folders and create virtual paths
		for ii = 1, #mint_shaders_to_load do
			shader = mint_shaders_to_load[ii]
			
			for jj = 1, #mint_shader_dirs do
				platform = mint_shader_dirs[jj]

				print("Loading shader: " .. "[" .. shader.."]" .. " = " .. mint_abs_path .. "/" .. mint_shader_dir_location .. "/" .. platform .. "/" .. shader)
				
				-- Create virtual path in solution
				vfldr = platform .. "/" .. shader
				vfldr_path = mint_abs_path .. "/" .. mint_shader_dir_location 
				print("Create Virtual folder: [" .. vfldr .. "] = " .. vfldr_path)
				vpaths{
					[vfldr] = { vfldr_path .. "/**.h", vfldr_path .. "/**.cpp"}
				}
			
				-- Load files to project
				files{
					vfldr_path .. "/" ..  vfldr .. "/**.h", vfldr_path .. "/" ..  vfldr .. "/**.cpp"
				}

				-- Allow including folder headers across project
				print("Create Include folder: " .. platform)
				includedirs{
					platform
				}
			end
		end
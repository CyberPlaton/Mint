function LinkLibraryForPlatformAndConfig(platform, config, library, library_path)
	print("[".. platform .."][".. config.."]Linking Library: \"" .. library .. "\" at " .. library_path)

	filter {"platforms:".. platform, "configurations:" .. config}
		libdirs{"libs", library_path}
		links{
			library
		}

	filter{}
end

function windows_linking()
	filter {"platforms:Windows", "configurations:Debug"}
		LinkLibraryForPlatformAndConfig("Windows", "Debug", "Mint", mint_dir .. "/Debug_Windows/Mint")
	filter {"platforms:Windows", "configurations:Release"}
		LinkLibraryForPlatformAndConfig("Windows", "Release", "Mint", mint_dir .. "/Release_Windows/Mint")
	filter {"platforms:Windows", "configurations:Distr"}
		LinkLibraryForPlatformAndConfig("Windows", "Distr", "Mint", mint_dir .. "/Distr_Windows/Mint")
end

function linux_linking()
	filter {"platforms:Linux", "configurations:Debug"}
		LinkLibraryForPlatformAndConfig("Linux", "Debug", "Mint", mint_dir .. "/Debug_Linux/Mint")
	filter {"platforms:Linux", "configurations:Release"}
		LinkLibraryForPlatformAndConfig("Linux", "Release", "Mint", mint_dir .. "/Release_Linux/Mint")
	filter {"platforms:Linux", "configurations:Distr"}
		LinkLibraryForPlatformAndConfig("Linux", "Distr", "Mint", mint_dir .. "/Distr_Linux/Mint")
end


mint_dir = os.getcwd()
mint_dir = mint_dir .. "/bin"

-----------------------------------------------------------
-- Sandbox: Platform specific linking of the engine library
-----------------------------------------------------------
if os.target() == "linux" then
	linux_linking()
elseif os.target() == "windows" then
	windows_linking()
elseif os.target() == "android" then
	print("Building for android is not supported!\n")
	os.exit()
elseif os.target() == "macosx" then 
	print("Building for macosx is not supported!\n")
	os.exit()
elseif os.target() == "ios" then
	print("Building for ios is not supported!\n")
	os.exit()
end

function LinkLibraryForPlatformAndConfig(platform, config, library, library_path)
	print("[".. platform .."][".. config.."]Linking Library: \"" .. library .. "\" at " .. library_path)

	filter {"platforms:".. platform, "configurations:" .. config}
		libdirs{"libs", library_path}
		links{
			library
		}

	filter{}
end

filter {"platforms:Windows", "configurations:Debug"}
	LinkLibraryForPlatformAndConfig("Windows", "Debug", "Mint", "bin/Debug_Windows")


filter {"platforms:Windows", "configurations:Release"}
	LinkLibraryForPlatformAndConfig("Windows", "Release", "Mint", "bin/Release_Windows")


filter {"platforms:Windows", "configurations:Distr"}
	LinkLibraryForPlatformAndConfig("Windows", "Distr", "Mint", "bin/Distr_Windows")

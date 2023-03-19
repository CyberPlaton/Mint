function CreateBinaryAndIntermediateOutputDirectory(is_binary, configuration, platform)
	if is_binary then
		targetdir ("bin/" .. configuration .. "_" .. platform .. "/%{prj.name}")
	else
		objdir ("intermediate/" .. configuration .. "_" .. platform .. "/%{prj.name}")
	end		
end

function LinkLibraryForPlatformAndConfig(platform, config, library, library_path)
	print("[".. platform .."][".. config.."]Linking Library: \"" .. library .. "\" at " .. library_path)

	filter {"platforms:".. platform, "configurations:" .. config}
		libdirs{"libs", library_path}
		links{
			library
		}

	filter{}
end


-----------------------------------------------------------
-- Windows defines
-----------------------------------------------------------
filter "platforms:Windows"
	system "windows"

	filter "Debug"
		optimize "Off"
		symbols "On"
		defines{
			"BX_CONFIG_DEBUG=1",
			"MINT_DEBUG",
			"_USE_MATH_DEFINES",
			"__STDC_FORMAT_MACROS",
			"_ITERATOR_DEBUG_LEVEL=2",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_SECURE_NO_DEPRECATE",
		}
		CreateBinaryAndIntermediateOutputDirectory(true, "Debug", "Windows")
		CreateBinaryAndIntermediateOutputDirectory(false, "Debug", "Windows")

	filter "Release"
		optimize "Full"
		symbols "On"
		defines{
			"BX_CONFIG_DEBUG=0",
			"MINT_RELEASE",
			"__STDC_FORMAT_MACROS",
			"_ITERATOR_DEBUG_LEVEL=0",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_SECURE_NO_DEPRECATE",
		}

		CreateBinaryAndIntermediateOutputDirectory(true, "Release", "Windows")
		CreateBinaryAndIntermediateOutputDirectory(false, "Release", "Windows")

	filter "Distr"
		optimize "Full"
		symbols "Off"
		defines{
			"BX_CONFIG_DEBUG=0",
			"MINT_DISTR",
			"__STDC_FORMAT_MACROS",
			"_ITERATOR_DEBUG_LEVEL=0",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_SECURE_NO_DEPRECATE",
		}
		CreateBinaryAndIntermediateOutputDirectory(true, "Distr", "Windows")
		CreateBinaryAndIntermediateOutputDirectory(false, "Distr", "Windows")


-----------------------------------------------------------
-- Linux defines
-----------------------------------------------------------
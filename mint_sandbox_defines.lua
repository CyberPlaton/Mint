include "utility.lua"

function windows_defines()
-----------------------------------------------------------
-- Windows defines
-----------------------------------------------------------
	filter "platforms:Windows"
		system "windows"
	
		filter "Debug"
			optimize "Off"
			symbols "On"
			defines{
				"MINT_DEBUG=1",
				"MINT_RELEASE=0",
				"MINT_DISTR=0",
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
				"MINT_DEBUG=0",
				"MINT_RELEASE=1",
				"MINT_DISTR=0",
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
				"MINT_DEBUG=0",
				"MINT_RELEASE=0",
				"MINT_DISTR=1",
				"__STDC_FORMAT_MACROS",
				"_ITERATOR_DEBUG_LEVEL=0",
				"_CRT_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_DEPRECATE",
			}
			CreateBinaryAndIntermediateOutputDirectory(true, "Distr", "Windows")
			CreateBinaryAndIntermediateOutputDirectory(false, "Distr", "Windows")
end


function linux_defines()
-----------------------------------------------------------
-- Linux defines
-----------------------------------------------------------
	filter "platforms:Linux"
		system "linux"
	
		filter "Debug"
			optimize "Off"
			symbols "On"
			defines{
				"__STDC_FORMAT_MACROS",
				"_ITERATOR_DEBUG_LEVEL=2",
				"_CRT_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_DEPRECATE",
			}
			CreateBinaryAndIntermediateOutputDirectory(true, "Debug", "Linux")
			CreateBinaryAndIntermediateOutputDirectory(false, "Debug", "Linux")
	
		filter "Release"
			optimize "Full"
			symbols "On"
			defines{
				"__STDC_FORMAT_MACROS",
				"_ITERATOR_DEBUG_LEVEL=0",
				"_CRT_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_DEPRECATE",
			}
	
			CreateBinaryAndIntermediateOutputDirectory(true, "Release", "Linux")
			CreateBinaryAndIntermediateOutputDirectory(false, "Release", "Linux")
	
		filter "Distr"
			optimize "Full"
			symbols "Off"
			defines{
				"__STDC_FORMAT_MACROS",
				"_ITERATOR_DEBUG_LEVEL=0",
				"_CRT_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_DEPRECATE",
			}
			CreateBinaryAndIntermediateOutputDirectory(true, "Distr", "Linux")
			CreateBinaryAndIntermediateOutputDirectory(false, "Distr", "Linux")
end



-----------------------------------------------------------
-- Sandbox: Platform specific defines
-----------------------------------------------------------
if os.target() == "linux" then
	linux_defines()
elseif os.target() == "windows" then
	windows_defines()
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
include "utility.lua"


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
			"MINT_RELEASE=1",
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
			"MINT_DISTR=1",
			"__STDC_FORMAT_MACROS",
			"_ITERATOR_DEBUG_LEVEL=0",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_SECURE_NO_DEPRECATE",
		}
		CreateBinaryAndIntermediateOutputDirectory(true, "Distr", "Windows")
		CreateBinaryAndIntermediateOutputDirectory(false, "Distr", "Windows")


-----------------------------------------------------------
-- Linux linking
-----------------------------------------------------------
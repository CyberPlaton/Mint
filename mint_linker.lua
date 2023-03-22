-----------------------------------------------------------
-- Windows linking
-----------------------------------------------------------
filter {"platforms:Windows","configurations:Debug"}
	libdirs{"libs", "%{prj.location}/common/lib/Debug"}
	links{
		"SDL3",
		"bgfxDebug",
		"bxDebug",
		"bimgDebug",
		"bimg_decodeDebug",
		"fmtd",
		"spdlogd",
		"box2d",
		"Lua52",

		"gdi32", 
		"kernel32",
		"psapi",
	}


filter {"platforms:Windows","configurations:Release"}
	libdirs{"libs", "%{prj.location}/common/lib/Release"}
	links{
		"SDL3",
		"bgfxRelease",
		"bxRelease",
		"bimgRelease",
		"bimg_decodeRelease",
		"fmt",
		"spdlog",
		"box2d",
		"Lua52",

		"gdi32", 
		"kernel32",
		"psapi",
	}


filter {"platforms:Windows","configurations:Distr"}
	libdirs{"libs", "%{prj.location}/common/lib/Distr"}
	links{
		"SDL3",
		"bgfxRelease",
		"bxRelease",
		"bimgRelease",
		"bimg_decodeRelease",
		"fmt",
		"spdlog",
		"box2d",
		"Lua52",

		"gdi32", 
		"kernel32",
		"psapi",
	}

-----------------------------------------------------------
-- Linux linking
-----------------------------------------------------------

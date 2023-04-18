-----------------------------------------------------------
-- Windows linking
-----------------------------------------------------------
filter {"platforms:Windows","configurations:Debug"}
	libdirs{"libs", "%{prj.location}/common/lib/Debug"}
	links{
		"raylib",
		"fmtd",
		"spdlogd",
		"Lua52",

		"gdi32", 
		"kernel32",
		"psapi",
	}


filter {"platforms:Windows","configurations:Release"}
	libdirs{"libs", "%{prj.location}/common/lib/Release"}
	links{
		"raylib",
		"fmt",
		"spdlog",
		"Lua52",

		"gdi32", 
		"kernel32",
		"psapi",
	}


filter {"platforms:Windows","configurations:Distr"}
	libdirs{"libs", "%{prj.location}/common/lib/Distr"}
	links{
		"raylib",
		"fmt",
		"spdlog",
		"Lua52",

		"gdi32", 
		"kernel32",
		"psapi",
	}

-----------------------------------------------------------
-- Linux linking
-----------------------------------------------------------

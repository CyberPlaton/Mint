
function windows_linking()
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
			"fmodL_vc",
	
			"opengl32",
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
			"fmod_vc",
	
			"opengl32",
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
			"fmod_vc",
	
			"opengl32",
			"gdi32", 
			"kernel32",
			"psapi",
		}
end

function linux_linking()
-----------------------------------------------------------
-- Linux linking
-----------------------------------------------------------
	filter {"platforms:Linux","configurations:Debug"}
		libdirs{"libs", "%{prj.location}/common/lib/Debug"}
		links{
			"raylib",
			"fmtd",
			"spdlogd",
			"Lua52",
	
			"GL",
			"X11", 
			"phtread",
		}
	
	
	filter {"platforms:Linux","configurations:Release"}
		libdirs{"libs", "%{prj.location}/common/lib/Release"}
		links{
			"raylib",
			"fmt",
			"spdlog",
			"Lua52",
	
			"GL",
			"X11", 
			"phtread",
		}
	
	
	filter {"platforms:Linux","configurations:Distr"}
		libdirs{"libs", "%{prj.location}/common/lib/Distr"}
		links{
			"raylib",
			"fmt",
			"spdlog",
			"Lua52",
	
			"GL",
			"X11", 
			"phtread",
		}
end


-----------------------------------------------------------
-- Engine platform specific linking
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
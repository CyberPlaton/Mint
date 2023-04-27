workspace "Mint Engine"
	print("\n")
	print("Creating Workspace: \"Mint Engine\":")
	startproject "Sandbox"
	architecture "x86_64"
	configurations{
		"Debug",		-- Traditional Debugging build
		"Release",		-- Verbose build to be debugged in a "Release" environment
		"Distr"			-- Shipping build
	}
	
	platforms{
		"Windows", "Linux"
	}

	if os.target() == "windows" then
		system "Windows"
	elseif os.target() == "linux" then
		system "Linux"
	end

	flags{
		"MultiProcessorCompile",
	}

	group "Mint"
		include "mint_project.lua"
	group ""

	group "Sandbox"
		include "mint_sandbox_project.lua"
	group ""
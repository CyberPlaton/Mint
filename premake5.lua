workspace "Mint Engine"
	print("\n")
	print("Creating Workspace: \"Mint Engine\":")
	architecture "x86_64"
	startproject "Sandbox"
	configurations{
		"Debug",		-- Traditional Debugging build
		"Release",		-- Verbose build to be debugged in a "Release" environment
		"Distr"			-- Shipping build
	}
	
	platforms{
		"Windows", "Linux"
	}

	flags{
		"MultiProcessorCompile"
	}

	group "Mint"
		include "mint_project.lua"
	group ""

	group "Sandbox"
		include "mint_sandbox_project.lua"
	group ""
#include "UI.h"


namespace mint
{

	CUI* CUI::s_CUI = nullptr;


	bool CUI::initialize(Window* window)
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontDefault();

		ImGui::StyleColorsDark();

#if BX_PLATFORM_WINDOWS or BX_PLATFORM_XBOXONE
		ImGui_ImplSDL3_InitForD3D(window);
#endif

		ImGui_Implbgfx_Init(255);


		return true;
	}


	void CUI::terminate()
	{
		ImGui_Implbgfx_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}


	void CUI::begin()
	{
		ImGui_Implbgfx_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}


	void CUI::end()
	{
		ImGui::Render();
		ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
	}


}
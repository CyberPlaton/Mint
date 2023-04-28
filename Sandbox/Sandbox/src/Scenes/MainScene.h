#include "Mint.h"


class CMainScene : public mint::CScene
{
public:
	MINT_DEFINE_SCENE_CTOR(CMainScene);


	void on_update(mint::f32 dt = 0.0f) override final;

	void on_ui_render(mint::f32 dt = 0.0f) override final;

	bool on_before_load() override final;

	bool on_load() override final;

	void on_before_unload() override final;

	void on_unload() override final;


	entt::entity m_knight;
};
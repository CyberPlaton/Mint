#include "Mint.h"


struct MyComponent INHERITS(mint::reflection::SBase)
{
	REFLECTED_CLASS(MyComponent);


	REFLECTED_MEMBER(mint::u64, VariantType_Uint64, m_entity);
	REFLECTED_MEMBER(mint::String, VariantType_String, m_name);
	REFLECTED_MEMBER(bool, VariantType_Boolean, m_isReflected);
};



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
#include "Mint.h"


struct MyComponent INHERITS(mint::reflection::SBase)
{
	REFLECTED_CLASS(MyComponent);


	REFLECTED_MEMBER(mint::u64,		VariantType_Uint64,		m_entity);
	REFLECTED_MEMBER(mint::String,	VariantType_String,		m_name);
	REFLECTED_MEMBER(bool,			VariantType_Boolean,	m_isReflected);
	REFLECTED_MEMBER(mint::u32,		VariantType_Uint32,		m_memberValue);
};


class TestWorldQueryFilter : public mint::world::CWorldQueryFilter
{
public:
	bool does_proxy_pass_filter(const mint::world::SWorldQueryProxy& proxy) override final
	{
		auto entity = proxy.m_entity;

		return CUCA::identifier_get_debug_name(entity) == "Entity_2";
	}

};


class TestWorldQueryDatabaseFilter : public mint::world::CWorldQueryDatabaseFilter
{
public:
	TestWorldQueryDatabaseFilter(const mint::String& label) : m_label(label)
	{
	}

	bool does_edge_pass_filter(mint::world::CEdge* edge) override final 
	{
		return edge->get_label() == m_label;
	}

private:
	mint::String m_label = 0;
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

	bool is_persistent() override final { return true; }


	void setup_test_database(mint::world::CDatabase& db);
	void perform_all_database_tests();
	void perform_database_test(mint::world::CDatabase& db, mint::Vector< mint::world::SToken >& bytecode, const mint::String& description);


	entt::entity m_knight;
};
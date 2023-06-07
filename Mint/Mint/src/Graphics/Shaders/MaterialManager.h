#ifndef _MINTFX_MATERIAL_MANAGER_H_
#define _MINTFX_MATERIAL_MANAGER_H_


#include "Common/Common.h"
#include "Materials/Material.h"
#include "Utility/STL/Map2.h"
#include "Utility/Serialization/Serializer.h"



namespace mint::fx
{
	class CMaterialManager
	{
	public:
		STATIC_GET(CMaterialManager, s_CMaterialManager);

		bool initialize();

		void terminate();

		void reset();

		bool add_material_for_entity(entt::entity entity, const SMaterialDefinition& material_definition);

		bool set_material_for_entity(const String& material_name, entt::entity entity);

		void remove_material_from_entity(entt::entity entity, const String& material_name);
		
		mint::CMap2< CMaterial >& get_materials_for_entity(entt::entity entity);

		CMaterial* get_main_material_for_entity(entt::entity entity);

		void set_default_main_material_for_entity(entt::entity entity, const String& default_texture = "DefaultSprite", const String& default_shader = "Sprite",
												  BlendMode blending_mode = BLEND_ALPHA, BlendingEquation blending_equation = BlendingEquation_BlendColor, 
												  BlendingFactor blending_src_factor = BlendingFactor_SrcAlpha, BlendingFactor blending_dst_factor = BlendingFactor_OneMinusSrcAlpha);

		void create_material_prefab(const String& material_name, const String& material_file_path);


	private:
		static CMaterialManager* s_CMaterialManager;

		MINT_CRITICAL_SECTION(m_criticalSection);

		std::map< EntityHandle, CMap2< CMaterial > > m_materials;

		CMap< std::pair< String, String > > m_materialPrefabs;


	private:
		void _load_uniforms(CMap< mint::fx::SShaderUniform >& uniforms, Vector< maml::SProperty >& properties);
	
	
	};

}


#endif
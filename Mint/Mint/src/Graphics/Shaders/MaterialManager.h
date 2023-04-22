#ifndef _MINTFX_MATERIAL_MANAGER_H_
#define _MINTFX_MATERIAL_MANAGER_H_


#include "Common/Common.h"
#include "Materials/Material.h"


namespace mint::fx
{
	class CMaterialManager
	{
	public:
		STATIC_GET(CMaterialManager, s_CMaterialManager);

		bool initialize();

		void terminate();

		void reset();

		void add_material_for_entity(entt::entity entity, const CMaterial& material);
		
		const Vector< CMaterial >& get_materials_for_entity(entt::entity entity) const;

	private:
		static CMaterialManager* s_CMaterialManager;

		MINT_CRITICAL_SECTION(m_criticalSection);

		CMap< Vector< CMaterial > > m_materials;

	};

}


#endif
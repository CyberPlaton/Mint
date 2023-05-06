#if MINT_DISTR
#else
#include "EntityMetaclassDatabase.h"


namespace mint::reflection
{
	mint::reflection::CEntityMetaclassDatabase* CEntityMetaclassDatabase::s_CEntityMetaclassDatabase = nullptr;


	CEntityMetaclassDatabase& CEntityMetaclassDatabase::add_entity_metaclass(u64 entity, CMetaClass* metaclass)
	{
		MINT_ASSERT(entity != SCAST(u64, entt::null), "Invalid operation. Invalid entity provided!");

		if (!m_database.lookup(entity))
		{
			m_database.add(entity, Vector< CMetaClass* >{});
		}

		auto& vector = m_database.get_ref(entity);

		mint::algorithm::vector_push_back(vector, metaclass);

		return *this;
	}

	mint::reflection::CEntityMetaclassDatabase& CEntityMetaclassDatabase::remove_entity_from_database(u64 entity)
	{
		MINT_ASSERT(entity != SCAST(u64, entt::null), "Invalid operation. Invalid entity provided!");

		if (m_database.lookup(entity))
		{
			m_database.remove(entity);
		}

		return*this;
	}


	mint::u32 CEntityMetaclassDatabase::get_metaclass_count(u64 entity)
	{
		MINT_ASSERT(entity != SCAST(u64, entt::null), "Invalid operation. Invalid entity provided!");

		if (m_database.lookup(entity))
		{
			return m_database.get_const(entity).size();
		}

		return 0;
	}


	mint::Vector< CMetaClass* >& CEntityMetaclassDatabase::get_entity_metaclasses(u64 entity)
	{
		MINT_ASSERT(entity != SCAST(u64, entt::null), "Invalid operation. Invalid entity provided!");

		if (m_database.lookup(entity))
		{
			return m_database.get_ref(entity);
		}

		return Vector< CMetaClass* >{};
	}


}
#endif
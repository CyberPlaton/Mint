#if MINT_DISTR
#else
#include "EntityMetaclassDatabase.h"


namespace mint::reflection
{
	mint::reflection::CEntityMetaclassDatabase* CEntityMetaclassDatabase::s_CEntityMetaclassDatabase = nullptr;


	CEntityMetaclassDatabase& CEntityMetaclassDatabase::add_entity_metaclass(u64 entity, CMetaClass* metaclass)
	{
		MINT_ASSERT(entity != SCAST(u64, entt::null), "Invalid operation. Invalid entity provided!");


		MINT_LOG_WARN("[{:.4f}][CEntityMetaclassDatabase::add_entity_metaclass] Adding Metaclass for Entity: \"{}\"\n\t\tMetaclass Name: \"{}\", Type: \"{}\", Entity: \"{}\"", 
					   MINT_APP_TIME, entity, metaclass->get_metaclass_name(), metaclass->get_metaclass_type(), (u64)metaclass->get_metaclass_entity());


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


	bool CEntityMetaclassDatabase::initialize()
	{
		return true;
	}


	void CEntityMetaclassDatabase::terminate()
	{
		reset();
	}


	void CEntityMetaclassDatabase::reset()
	{
		m_database.reset();
	}


	void CEntityMetaclassDatabase::reset(Vector< entt::entity >& entities)
	{
		for(auto& entity : entities)
		{
			remove_entity_from_database(SCAST(u64, entity));
		}
	}


}
#endif
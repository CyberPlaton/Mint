#if MINT_DISTR
#else 
#ifndef _MINT_ENTITY_META_CLASS_DATABASE_H_
#define _MINT_ENTITY_META_CLASS_DATABASE_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"
#include "Metaclass.h"
#include "Utility/Logging/Logging.h"


namespace mint::reflection
{

	class CEntityMetaclassDatabase
	{
	public:
		STATIC_GET(CEntityMetaclassDatabase, s_CEntityMetaclassDatabase);

		bool initialize();

		void terminate();

		void reset();

		void reset(Vector< entt::entity >& entities);


		CEntityMetaclassDatabase& add_entity_metaclass(u64 entity, CMetaClass* metaclass);

		template < typename T >
		CEntityMetaclassDatabase& remove_entity_metaclass(u64 entity);

		CEntityMetaclassDatabase& remove_entity_from_database(u64 entity);

		u32 get_metaclass_count(u64 entity);

		Vector< CMetaClass* >& get_entity_metaclasses(u64 entity);



	private:
		static CEntityMetaclassDatabase* s_CEntityMetaclassDatabase;

		CMap< Vector< CMetaClass* > > m_database;

	};


	template < typename T >
	CEntityMetaclassDatabase& mint::reflection::CEntityMetaclassDatabase::remove_entity_metaclass(u64 entity)
	{
		MINT_ASSERT(entity != SCAST(u64, entt::null), "Invalid operation. Invalid entity provided!");

		if (m_database.lookup(entity))
		{
			auto metaclass_identifier = entt::type_id< T >().hash();
			auto& vector = m_database.get_ref(entity);

			u32 index = 0;
			for (auto& mc : vector)
			{
				if (mc->get_metaclass_type() == metaclass_identifier)
				{
					mint::algorithm::vector_erase_at(vector, index);
					break;
				}
				index++;
			}
		}

		return*this;
	}


}


#endif
#endif
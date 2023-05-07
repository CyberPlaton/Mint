#include "Registry.h"


namespace mint
{


	CRegistry::CRegistry()
	{
		initialize();
	}


	CRegistry::~CRegistry()
	{
		terminate();
	}


	bool CRegistry::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void CRegistry::terminate()
	{
		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CRegistry::reset()
	{
		auto view = get_registry_view< mint::component::SIdentifier >();

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_registry.destroy(view.begin(), view.end());

			m_registry.clear();

			);
	}


	void CRegistry::reset(Vector< entt::entity > entities)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_registry.destroy(entities.begin(), entities.end());

			m_registry.clear();

			);
	}


	entt::entity CRegistry::create_entity(u64 identifier_hint /*= MINT_INVALID_HANDLE*/)
	{
		entt::entity entity = entt::null;

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			if (identifier_hint == MINT_INVALID_HANDLE)
			{
				entity = m_registry.create();
			}
			else
			{
				entity = m_registry.create(SCAST(entt::entity, identifier_hint));
			}

		);

		return entity;
	}


	void CRegistry::delete_entity(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_registry.destroy(entity);

		);


#if MINT_DISTR
#else
		mint::reflection::CEntityMetaclassDatabase::Get().remove_entity_from_database(SCAST(u64, entity));
#endif
	}


	const entt::registry& CRegistry::get_entt_registry()
	{
		return m_registry;
	}


	entt::registry& CRegistry::get_entt_registry_ref()
	{
		return m_registry;
	}


}
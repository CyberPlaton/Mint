#ifndef _MINT_REGISTRY_H_
#define _MINT_REGISTRY_H_


#include "Common/Common.h"
#include "Components/Components.h"


namespace mint
{
	class CRegistry
	{
	public:
		CRegistry();
		~CRegistry();


		bool initialize();

		void terminate();

		void reset();

		void reset(Vector< entt::entity > entities);


		entt::entity create_entity(u64 identifier_hint = MINT_INVALID_HANDLE);

		void delete_entity(entt::entity entity);


		template< typename T >
		T& get_component(entt::entity entity);

		template< typename... T >
		bool has_component(entt::entity entity);

		template< typename T >
		T& add_component(entt::entity entity);

		template< typename T >
		void remove_component(entt::entity entity);


		template< typename T >
		auto get_registry_view()
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto view = m_registry.view< T >();
			);

			return view;
		}


		template < typename... T >
		auto get_registry_group()
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto group = m_registry.group< T... >();
			);

			return group;
		}


	private:
		MINT_CRITICAL_SECTION(m_criticalSection);

		entt::registry m_registry;

	};


	template< typename T >
	void mint::CRegistry::remove_component(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_registry.erase< T >(entity);

		);
	}


	template< typename T >
	T& mint::CRegistry::add_component(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto& component = m_registry.emplace< T >(entity);

		);

		return component;
	}


	template< typename... T >
	bool mint::CRegistry::has_component(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			auto has = m_registry.all_of< T... >(entity);

		);

		return has;
	}


	template< typename T >
	T& mint::CRegistry::get_component(entt::entity entity)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			T& component = m_registry.get< T >(entity);

		);

		return component;
	}



}



#endif
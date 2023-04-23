#ifndef _MINT_SERVICE_LOCATOR_H_
#define _MINT_SERVICE_LOCATOR_H_


#include "Service.h"


namespace mint
{

	class CServiceLocator
	{
	public:
		STATIC_GET(CServiceLocator, s_CServiceLocator);

		bool initialize();

		void terminate();

		template< typename T >
		T* locate_service(const String& service_type);


	private:
		static CServiceLocator* s_CServiceLocator;

		MINT_CRITICAL_SECTION(m_criticalSection);
	};


	template< typename T >
	T* mint::CServiceLocator::locate_service(const String& service_type)
	{
		auto h = mint::algorithm::djb_hash(service_type);

		if(IService::g_Services.lookup(h))
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto service = dynamic_cast<T*>(IService::g_Services.get(h));

			);

			return service;
		}

		return nullptr;
	}


}

#endif
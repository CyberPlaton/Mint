#include "Service.h"


namespace mint
{
	MINT_CRITICAL_SECTION(IService::m_criticalSection);

	mint::CMap< IService* > IService::g_Services;


	bool IService::register_service(IService* service, const String& service_type)
	{
		if(service->initialize())
		{
			auto h = mint::algorithm::djb_hash(service_type);

			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				g_Services.add(h, service);

			);

			return true;
		}

		
		delete service; service = nullptr;
		
		return false;
	}


	void IService::unregister_service(const String& service_type)
	{
		auto h = mint::algorithm::djb_hash(service_type);

		if (g_Services.lookup(h))
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto service = g_Services.get(h);

				g_Services.remove(h);

			);

			service->terminate();

			delete service; service = nullptr;
		}
	}


	void IService::terminate_services()
	{
		auto& services = g_Services.get_all();

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			while (!services.empty())
			{
				unregister_service(services[0]->get_service_type());
			}

		);
	}

	void IService::print_registered_services()
	{
		const auto& services = g_Services.get_all();

		MINT_LOG_INFO("[{:.4f}][IService::print_registered_services] Dumping registered services:", MINT_APP_TIME);

		for(const auto service : services)
		{
			MINT_LOG_INFO("\tService Type: \"{}\"", service->get_service_type());
		}
	}


	bool IService::_initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void IService::_terminate()
	{
		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


}
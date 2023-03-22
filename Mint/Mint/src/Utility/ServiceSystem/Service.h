#ifndef _MINT_SERVICE_H_
#define _MINT_SERVICE_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"

#include "../STL/Map.h"


namespace mint
{
	class CServiceLocator;


	class IService
	{
		friend class CServiceLocator;

	public:
		static bool register_service(IService* service, const String& service_type);

		static void unregister_service(const String& service_type);

		static void terminate_services();



		virtual bool initialize() = 0;

		virtual void terminate() = 0;

		virtual void reset() = 0;

		virtual String get_service_type() = 0;


	private:
		static CMap< IService* > g_Services;

		static MINT_CRITICAL_SECTION(m_criticalSection);

	private:
		static bool _initialize();

		static void _terminate();

	};

}


#define MINT_REGISTER_SERVICE(class_name, service) \
mint::IService::register_service(new class_name(), service)


#endif
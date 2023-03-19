#include "ServiceLocator.h"


namespace mint
{


	bool CServiceLocator::initialize()
	{
		IService::_initialize();

		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void CServiceLocator::terminate()
	{
		IService::_terminate();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


}
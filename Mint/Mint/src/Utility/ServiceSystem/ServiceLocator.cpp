#include "ServiceLocator.h"


namespace mint
{

	mint::CServiceLocator* CServiceLocator::s_CServiceLocator = nullptr;


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
#include "IMint.h"

namespace mint
{
	mint::IMintEngine* IMintEngine::s_engine = nullptr;


	mint::IMintEngine* IMintEngine::get_engine()
	{
		return s_engine;
	}


	void IMintEngine::initialize_cuca_critical_sections()
	{
		INITIALIZE_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	}


	void IMintEngine::delete_cuca_critical_sections()
	{
		DELETE_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	}


	void IMintEngine::run_spatial_acceleration_structure()
	{
		CSAS::Get()._run();
	}


	void IMintEngine::run_scripting_system()
	{

	}


}
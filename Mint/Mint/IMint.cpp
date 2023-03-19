#include "IMint.h"

namespace mint
{
	mint::IMintEngine* IMintEngine::s_engine = nullptr;


	mint::IMintEngine* IMintEngine::get_engine()
	{
		return s_engine;
	}


	void IMintEngine::run_event_system()
	{
		CEventSystem::Get()._run();
	}


	void IMintEngine::run_spatial_acceleration_structure()
	{
		CSAS::Get()._run();
	}


	void IMintEngine::run_scripting_system()
	{

	}


}
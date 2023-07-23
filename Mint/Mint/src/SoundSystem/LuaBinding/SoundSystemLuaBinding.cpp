#include "SoundSystemLuaBinding.h"


namespace mint::luaglue
{
	namespace script
	{
		bool sound_system_common(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Sound")
				.endNamespace();

			return true;
		}


	}

	namespace behavior
	{

		bool sound_source(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)
				.beginNamespace("Sound")
				.endNamespace();
			
			return true;
		}


	}
}


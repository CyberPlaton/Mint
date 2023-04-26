#include "LuaComponentsModule.h"

namespace mint::luaglue
{
	namespace script
	{

		bool luamodule_components(lua_State* state)
		{
			bool initialization = true;

			initialization &= behavior::identifier(state);

			initialization &= behavior::scene_hierarchy(state);

			initialization &= behavior::transform(state);

			initialization &= behavior::sprite(state);

			initialization &= behavior::animated_sprite(state);

			return initialization;
		}


	}

	namespace behavior
	{


		bool luamodule_components(lua_State* state)
		{
			bool initialization = true;

			initialization &= identifier(state);

			initialization &= scene_hierarchy(state);

			initialization &= transform(state);

			initialization &= sprite(state);

			initialization &= animated_sprite(state);

			return initialization;
		}


	}
}